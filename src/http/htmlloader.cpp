#include "htmlloader.h"
#include "httpparser.h"

#include "callbackwithargument.h"
#include "callbackwithargumentanddata.h"

HtmlLoader::HtmlLoader(ICallback *callback, Folder *folder, Controller *controller, QObject *parent)
	: QObject(parent), _callback(callback), _controller(controller), _folder(folder)
{
	if(!folder)
		throw "Could not create HttpItemCollectionLoader. Folder must not be null!";

	if(!controller)
		throw "Could not create HttpItemCollectionLoader. Controller must not be null!";

	if(!callback)
		throw "Could not create HttpItemCollectionLoader. Callback must not be null!";

	controller->http()->get(folder->getUrl(), new CallbackWithArgument<HtmlLoader, QByteArray>(this, &HtmlLoader::parseHtml, _callback));
}

HtmlLoader::~HtmlLoader()
{
	_callback = nullptr;
	_controller = nullptr;
	_folder = nullptr;
	_itemsToLoadCount = 0;

	while(!_itemsToLoad.isEmpty())
		delete _itemsToLoad.takeLast();
}

void HtmlLoader::parseHtml(QByteArray content)
{
	_itemsToLoad = HttpParser::parse(_folder->getUrl(), content);
	_itemsToLoadCount = _itemsToLoad.count();

	if(_itemsToLoadCount > 0)
	{
		foreach (auto item, _itemsToLoad) {
			_controller->http()->getInfo(item->getUrl(),
				new CallbackWithArgumentAndData<HtmlLoader, QSharedPointer<RemoteResourceInfo>, HttpItem*>(this, item, &HtmlLoader::setResourceInfo, &HtmlLoader::resourceInfoFailed));
		}
	}
	else
	{
		_folder->setIsLoaded(true);
		_callback->success();
	}
}

void HtmlLoader::setResourceInfo(QSharedPointer<RemoteResourceInfo> resource, HttpItem *item)
{
	if(resource && item)
	{
		item->setMime(resource->mime);
		item->setSize(resource->size);
	}
	else if(item)
	{
		_itemsToLoad.removeOne(item);
	}

	tryFinish();
}

void HtmlLoader::resourceInfoFailed(QString error, HttpItem *item)
{
	if(item)
	{
		qWarning() << "Failed to load http item size: " << error << "Item will be remmoved.";
		_itemsToLoad.removeOne(item);
	}

	tryFinish();
}

void HtmlLoader::tryFinish()
{
	_itemsToLoadCount--;

	if(_itemsToLoadCount == 0)
	{
		{
			auto adder = _controller->contentContainer()->beginAdd(_folder);

			foreach (HttpItem *item, _itemsToLoad)
			{
				adder->addItem(item);
			}
		}

		_callback->success();
	}
}
