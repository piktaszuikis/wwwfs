#ifndef FILE_H
#define FILE_H

#include "contentitem.h"
#include <QByteArray>

class File : public ContentItem
{
public:
	File(fuse_ino_t node_id, QString name, size_t size, QUrl url);

	size_t size() const;
	void setSize(size_t size);

	QByteArray content(off_t off, size_t size) const;
	void setContent(const QByteArray content);

	// ContentItem interface
protected:
	void wfuse_apply_stat(struct stat *st) override;

private:
	size_t _size;
	QByteArray _content;
};

#endif // FILE_H
