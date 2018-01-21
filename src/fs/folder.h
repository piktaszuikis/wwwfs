#ifndef FOLDER_H
#define FOLDER_H

#include "contentitem.h"
#include <QList>

class Folder : public ContentItem
{
public:
	Folder(fuse_ino_t node_id, QString name, QUrl url);

	const QList<ContentItem *> contents() const;
	Folder *addContent(ContentItem * item);
	ContentItem *getByName(QString name);

	void clearCache();
	QByteArray wfuse_get_direntries();

private:
	QList<ContentItem *> _contents;
	QByteArray _fuseDirentryCache;

	// ContentItem interface
protected:
	void wfuse_apply_stat(struct stat *st) override;

};

#endif // FOLDER_H
