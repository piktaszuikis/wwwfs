#include "rendererbase.h"
#include "contentcontainer.h"

RendererBase::RendererBase(ContentContainer *container, Folder *parent)
{
	if(!container)
		throw "Renderer must own a container!";

	if(!parent)
		throw "Renderer must own a parent folder!";

	_container = container;
	_parent = parent;
}

ContentItem *RendererBase::addItem(HttpItem *item)
{
	if(item && !hasUrlInParent(item->getUrl()))
	{
		auto existing = _container->getByUrl(item->getUrl());
		if(existing)
		{
			//TODO: return symlink
		}

		if(isFolder(item))
			return addContent(createFolder(item));
		else
			return addContent(createFile(item));
	}

	return 0;
}

RendererBase::~RendererBase()
{
	_parent->setIsLoaded(true);

	_container = 0;
	_parent = 0;
}

fuse_ino_t RendererBase::nextID()
{
	return _container->nextID();
}

ContentItem *RendererBase::addContent(ContentItem *item)
{
	_parent->addContent(item);
	_container->addContent(item);
	return item;
}

QString RendererBase::getUniqueName(QString suggested)
{
	long long suffix = 0;

	suggested.replace('/', QChar(L'／')); //slashes are not allowed!
	QString result = suggested;

	while(hasName(result))
	{
		result = suggested + QString("_") + QString::number(suffix++);
	}

	return result;
}

bool RendererBase::hasName(QString name)
{
	if(name == "." || name == "..") //these names are reserved!
		return true;

	return _parent->getByName(name) != 0;
}

bool RendererBase::hasUrlInParent(QUrl url)
{
	foreach (ContentItem *item, _parent->contents())
	{
		if(item->getUrl().matches(url, QUrl::RemoveScheme | QUrl::RemoveFragment))
			return true;
	}

	return false;
}
