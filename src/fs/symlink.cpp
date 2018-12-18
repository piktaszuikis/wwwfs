#include <QDir>
#include <QDebug>

#include "symlink.h"

Symlink::Symlink(fuse_ino_t node_id, QString name, ContentItem *entry)
	: ContentItem (node_id, name, QUrl()), _entry(entry)
{

}

QString Symlink::link()
{
	if(_link.isEmpty() && _entry)
	{
		qDebug() << "Constructing link...";
		QString filename = _entry->filename();
		QString path = this->path();

		qDebug() << path << filename;

		if(!filename.isEmpty() && ! path.isEmpty())
			_link = QDir(path).relativeFilePath(filename);

		qDebug() << _link;
	}

	return _link;
}

void Symlink::wfuse_apply_stat(struct stat *st)
{
	st->st_mode = S_IFLNK| (S_IRUSR | S_IRGRP | S_IROTH);
	st->st_size = path().length();
	st->st_nlink = 1;
}
