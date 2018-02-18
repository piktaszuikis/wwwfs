#include <QAtomicInteger>
#include "contentcontainer.h"
#include "configurationmanager.h"

static QAtomicInteger<fuse_ino_t> _nodeID(FUSE_ROOT_ID);

ContentContainer::ContentContainer()
{
	addContent(new Folder(nextID(), ".", ConfigurationManager::rootUrl()));
}

ContentItem *ContentContainer::getByID(fuse_ino_t node_id)
{
	return _map.value(node_id); //if it does not exists, this will return 0
}

File *ContentContainer::getFileByID(fuse_ino_t node_id)
{
	return dynamic_cast<File *>(getByID(node_id));
}

Folder *ContentContainer::getFolderByID(fuse_ino_t node_id)
{
	return dynamic_cast<Folder *>(getByID(node_id));
}

ContentItem *ContentContainer::getByUrl(QUrl url)
{
	return _urlMap.value(url.adjusted(QUrl::RemoveFragment));
}

QSharedPointer<RendererBase> ContentContainer::beginAdd(Folder *parent)
{
	return QSharedPointer<RendererBase>(ConfigurationManager::createRenderer(this, parent));
}

void ContentContainer::addContent(ContentItem *item)
{
	if(item)
	{
		_map.insert(item->nodeID(), item);
		_urlMap.insert(item->getUrl().adjusted(QUrl::RemoveScheme | QUrl::RemoveFragment), item);
	}
}

fuse_ino_t ContentContainer::nextID()
{
	return _nodeID++;
}


