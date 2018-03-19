#include "contentitem.h"
#include "folder.h"

ContentItem::ContentItem(fuse_ino_t node_id, QString name, QUrl url)
	: _nodeID(node_id), _cachedStat(nullptr), _name(name), _url(url), _parent(nullptr)
{

}

ContentItem::~ContentItem()
{
	delete _cachedStat;
	_parent = nullptr;
}

QString ContentItem::name() const
{
	return _name;
}

void ContentItem::setName(const QString name)
{
	_name = name;
	clearCache();
}

Folder *ContentItem::parent() const
{
	return _parent;
}

void ContentItem::setParent(Folder *parent)
{
	_parent = parent;
}

QUrl ContentItem::getUrl() const
{
	return _url;
}

void ContentItem::setUrl(const QUrl url)
{
	_url = url;
}

fuse_ino_t ContentItem::nodeID() const
{
	return _nodeID;
}

void ContentItem::wfuse_stat(struct stat *st)
{
	if(st)
	{
		st->st_nlink = 1;
		st->st_ino = _nodeID;

		wfuse_apply_stat(st);
	}
}

void ContentItem::clearCache()
{
	if(parent())
		parent()->clearCache();

	//_cachedStat never changes
	//delete _cachedStat;
	//_cachedStat = 0;
}

struct stat *ContentItem::wfuse_stat()
{
	if(!_cachedStat)
	{
		_cachedStat = new struct stat();
		wfuse_apply_stat(_cachedStat);
	}

	return _cachedStat;
}
