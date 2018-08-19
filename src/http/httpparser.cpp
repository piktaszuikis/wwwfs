#include "httpparser.h"

#include <QString>
#include <QList>
#include <qgumbodocument.h>
#include <qgumbonode.h>
#include <functional>
#include "../configurationmanager.h"

//https://github.com/lagner/QGumboParser

class ExtractContext
{
public:
	ExtractContext(QUrl root)
	{
		_documentRoot = root;
		_addedUrls.append(_documentRoot.toString());
	}

	void addResult(HttpItem *item)
	{
		if(item && !wasAdded(item->getUrl()) && !isFiltered(item->getUrl()))
		{
			setWasAdded(item->getUrl());
			_result.append(item);
		}
	}

	QUrl resolved(QString url)
	{
		return _documentRoot.resolved(url).adjusted(QUrl::RemoveFragment);
	}

	bool isFiltered(const QUrl &url)
	{
		if(ConfigurationManager::isAvoidThumbnails())
		{
			if(url.path().contains("thumb", Qt::CaseInsensitive))
				return true;
		}

		return false;
	}

	bool wasAdded(const QUrl &url)
	{
		QString dbg = url.toString();
		return _addedUrls.contains(dbg);
	}

	void setWasAdded(const QUrl &url)
	{
		_addedUrls.append(url.toString());
	}

	QList<HttpItem *> takeResult()
	{
		QList<HttpItem *>result = _result;
		delete this;

		return result;
	}

private:
	QStringList _addedUrls;
	QList<HttpItem *> _result;
	QUrl _documentRoot;

};


void extractByTag(ExtractContext *context, const QGumboNode &root, HtmlTag tag,
				  std::function<HttpItem *(ExtractContext *, const QGumboNode &)> creator)
{
	auto nodes = root.getElementsByTagName(tag);

	for (const auto& node: nodes)
	{
		context->addResult(creator(context, node));
	}

	return;
}

HttpItem *createFromA(ExtractContext *context, const QGumboNode &node)
{
	QString href = node.getAttribute("href").trimmed();

	if(href.isEmpty())
		return nullptr;

	if(href.startsWith("javascript:"))
		return nullptr;

	if(href.startsWith("#"))
		return nullptr;

	QUrl url = context->resolved(href);

	if(context->wasAdded(url))
		return nullptr;

	return new HttpItem("a", node.innerText(), node.getAttribute("title"), url);
}

HttpItem *createFromImg(ExtractContext *context, const QGumboNode &node)
{
	QString src = node.getAttribute("src").trimmed();

	if(src.isEmpty())
		return nullptr;

	if(src.startsWith("data:")) //TODO: add support for data
		return nullptr;

	QUrl url = context->resolved(src);

	if(context->wasAdded(url))
		return nullptr;

	return new HttpItem("img", node.getAttribute("title"), node.getAttribute("alt"), url);
}

QList<HttpItem *> HttpParser::parse(const QUrl &url, QByteArray html)
{
	auto doc = QGumboDocument::parse(html);
	auto root = doc.rootNode();

	ExtractContext *context = new ExtractContext(url);

	//extractByTag(result, url, root, HtmlTag::SCRIPT);
	extractByTag(context, root, HtmlTag::A, &createFromA);
	extractByTag(context, root, HtmlTag::IMG, &createFromImg);

	return context->takeResult();
}
