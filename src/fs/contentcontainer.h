#ifndef CONTENTCONTAINER_H
#define CONTENTCONTAINER_H

#include <QString>
#include <QSharedPointer>
#include <QHash>

#include <fuse_lowlevel.h>
#include "folder.h"
#include "file.h"
#include "rendererbase.h"

class ContentContainer
{
public:
	ContentContainer();

	ContentItem *getByID(fuse_ino_t node_id);
	File *getFileByID(fuse_ino_t node_id);
	Folder *getFolderByID(fuse_ino_t node_id);

	ContentItem *getByUrl(QUrl url);

	QSharedPointer<RendererBase> beginAdd(Folder *parent);

protected:
	void addContent(ContentItem *item);
	fuse_ino_t nextID();

	friend ContentItem * RendererBase::addContent(ContentItem *);
	friend fuse_ino_t RendererBase::nextID();

private:
	QHash<fuse_ino_t, ContentItem*> _map;
	QHash<QUrl, ContentItem*> _urlMap;
};

#endif // CONTENTCONTAINER_H
