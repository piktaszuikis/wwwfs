#ifndef FILE_H
#define FILE_H

#include "contentitem.h"
#include <QByteArray>

class File : public ContentItem
{
public:
	File(fuse_ino_t node_id, QString name, size_t size, QUrl url);

	size_t size() const;

protected:
	void wfuse_apply_stat(struct stat *st) override;

private:
	size_t _size;
};

#endif // FILE_H
