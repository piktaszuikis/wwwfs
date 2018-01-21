#include "file.h"

File::File(fuse_ino_t node_id, QString name, size_t size, QUrl url)
	: ContentItem(node_id, name, url), _size(size)
{

}

size_t File::size() const
{
	if(isLoaded())
		return _content.size();

	return _size;
}

void File::setSize(size_t size)
{
	_size = size;
}

QByteArray File::content(off_t off, size_t size) const
{
	if(off > this->size())
		return QByteArray();

	return _content.mid(off, size);
}

void File::setContent(const QByteArray content)
{
	_content = content;
	clearCache();
}

void File::wfuse_apply_stat(struct stat *st)
{
	st->st_mode = S_IFREG | (S_IRUSR | S_IRGRP | S_IROTH);
	st->st_size = size();
}
