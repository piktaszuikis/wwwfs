#include "httpitemcollectionloader.h"
#include "ifileinfocallback.h"
#include "httpparser.h"
#include <QDebug>

class FileInfoLoader : public QObject, public IFileInfoCallback
{
public:
	FileInfoLoader(HttpItemCollectionLoader *parent, HttpItem *item);
	virtual ~FileInfoLoader();

	void setInfo(size_t size, QString mime) override;
	void setError(QString error) override;

private:
	HttpItemCollectionLoader *_parent;
	HttpItem *_item;
};

FileInfoLoader::FileInfoLoader(HttpItemCollectionLoader *parent, HttpItem *item)
	: QObject(parent)
{
	_parent = parent;
	_item = item;
}

FileInfoLoader::~FileInfoLoader()
{
	_parent = nullptr;
	_item = nullptr;
}

void FileInfoLoader::setInfo(size_t size, QString mime)
{
	if(_parent && _item)
		_parent->setItemInfo(_item, size, mime);
}

void FileInfoLoader::setError(QString error)
{
	if(_parent && _item)
		_parent->setItemError(_item, error);
}


HttpItemCollectionLoader::HttpItemCollectionLoader(Folder *folder, Controller *controller, QObject *parent)
	: QObject(0), _itemsToLoadCount(0)
{
	if(!folder)
		throw "Could not create HttpItemCollectionLoader. Folder must not be null!";

	if(!controller)
		throw "Could not create HttpItemCollectionLoader. Controller must not be null!";

	moveToThread(controller);
	setParent(parent);

	_controller = controller;
	_folder = folder;
}

HttpItemCollectionLoader::~HttpItemCollectionLoader()
{
	_itemsToLoadCount = 0;
	_folder = nullptr;
	_controller = nullptr;

	while(!_itemsToLoad.isEmpty())
		delete _itemsToLoad.takeLast();
}


void HttpItemCollectionLoader::finishLoading()
{
	/*using(add)*/
	{
		auto add = _controller->contentContainer()->beginAdd(_folder);

		if(_itemsToLoad.count() > 0)
		{
			foreach (HttpItem *item, _itemsToLoad)
			{
				add->addItem(item);
			}
		}
	}

	emit this->loaded(_folder);
}

void HttpItemCollectionLoader::setItemInfo(HttpItem *item, size_t size, QString mime)
{
	if(item)
	{
		if(size > 0)
			item->setSize(size);

		item->setMime(mime);

		_itemsToLoadCount--;

		if(_itemsToLoadCount == 0)
			finishLoading();
	}
}

void HttpItemCollectionLoader::setItemError(HttpItem *item, QString error)
{
	if(item)
	{
		qWarning() << "Failed to load http item size: " << error << "Item will be remmoved.";

		_itemsToLoad.removeOne(item);
		_itemsToLoadCount--;

		if(_itemsToLoadCount == 0)
			finishLoading();
	}
}

void HttpItemCollectionLoader::setContent(QUrl url, QByteArray content)
{
	_itemsToLoad = HttpParser::parse(url, content);
	_itemsToLoadCount = _itemsToLoad.count();

	if(_itemsToLoadCount > 0)
	{
		foreach(HttpItem *item, _itemsToLoad)
			_controller->http()->getContentInfo(item->getUrl(), new FileInfoLoader(this, item));
	}
	else
	{
		finishLoading();
	}
}

void HttpItemCollectionLoader::setError(QString error)
{
	qWarning() << "Failed to load folder (download HTML failed): " << error;
	emit this->error(_folder);
}
