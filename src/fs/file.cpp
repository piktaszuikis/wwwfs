#include "file.h"

File::File(fuse_ino_t node_id, QString name, size_t size, QUrl url)
	: ContentItem(node_id, name, url), _size(size)
{

}

size_t File::size() const
{
	return _size;
}

void File::wfuse_apply_stat(struct stat *st)
{
	st->st_mode = S_IFREG | (S_IRUSR | S_IRGRP | S_IROTH);
	st->st_size = size();
}
