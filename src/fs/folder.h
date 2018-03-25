#ifndef FOLDER_H
#define FOLDER_H

#include "contentitem.h"
#include <QList>

class Folder : public ContentItem
{
public:
	Folder(fuse_ino_t node_id, QString name, QUrl url);
	virtual ~Folder();

	const QList<ContentItem *> contents() const;
	Folder *addContent(ContentItem * item);

	ContentItem *getByName(QString name);

	bool isLoaded() const;
	void setIsLoaded(const bool isLoaded);

	void clearCache();
	QByteArray wfuse_get_direntries();

private:
	QList<ContentItem *> _contents;
	QByteArray _fuseDirentryCache;
	bool _isLoaded;

protected:
	void wfuse_apply_stat(struct stat *st) override;

};

#endif // FOLDER_H
