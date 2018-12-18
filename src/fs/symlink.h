#ifndef SYMLINK_H
#define SYMLINK_H

#include "contentitem.h"
#include "file.h"

class Symlink : public ContentItem
{
public:
	Symlink(fuse_ino_t node_id, QString name, ContentItem *entry);
	QString link();

protected:
	void wfuse_apply_stat(struct stat *st) override;

private:
	ContentItem *_entry;
	QString _link;
};

#endif // SYMLINK_H
