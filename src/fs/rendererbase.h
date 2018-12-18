#ifndef CONTENTRENDERERBASE_H
#define CONTENTRENDERERBASE_H

#include <fuse_lowlevel.h>
#include "../http/httpitem.h"
#include "folder.h"

class ContentContainer;

/*abstract*/ class RendererBase
{
	friend class ContentContainer;

public:
	RendererBase(ContentContainer* container, Folder *parent);
	virtual ContentItem *addItem(HttpItem *item);
	virtual ~RendererBase();

protected:
	fuse_ino_t nextID();
	ContentItem * addContent(ContentItem *item);

	virtual bool isFolder(HttpItem *item) = 0;
	virtual ContentItem *createFile(HttpItem *item) = 0;
	virtual ContentItem *createFolder(HttpItem *item) = 0;
	virtual ContentItem *createSymlink(HttpItem *item, ContentItem *existing) = 0;

	virtual QString getUniqueName(QString suggested);
	virtual bool hasName(QString name);

	virtual bool hasUrlInParent(QUrl url);

private:
	ContentContainer* _container;
	Folder *_parent;

};

#endif // CONTENTRENDERERBASE_H
