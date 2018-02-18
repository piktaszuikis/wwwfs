#include "folder.h"
#include <QByteArray>
#include <QPair>

struct FuseDirEntryBuf
{
	char* name;
	size_t size;
	struct stat stat;

	FuseDirEntryBuf(ContentItem *item, const char *name = 0)
	{
		if(name)
			this->name = strdup(name);
		else
			this->name = strdup(item->name().toUtf8().constData());

		this->size = fuse_add_direntry(0, NULL, 0, this->name, NULL, 0);
		this->stat = {};

		item->wfuse_stat(&this->stat);
	}

	~FuseDirEntryBuf(){ delete name; }
};

Folder::Folder(fuse_ino_t node_id, QString name, QUrl url)
	: ContentItem(node_id, name, url), _isLoaded(false)
{

}

const QList<ContentItem *> Folder::contents() const
{
	return _contents;
}

Folder *Folder::addContent(ContentItem *item)
{
	if(item)
	{
		_contents.append(item);
		item->setParent(this);
		clearCache();
	}

	return this;
}

ContentItem *Folder::getByName(QString name)
{
	if(name == ".")
		return this;

	if(name == "..")
	{
		return parent() ?: this;
	}

	foreach (ContentItem *item, contents()) {
		if(item->name() == name)
			return item;
	};

	return 0;
}

bool Folder::isLoaded() const
{
	return _isLoaded;
}

void Folder::setIsLoaded(const bool isLoaded)
{
	_isLoaded = isLoaded;
}

void Folder::clearCache()
{
	_fuseDirentryCache.clear();
}

QByteArray Folder::wfuse_get_direntries()
{
	if(_fuseDirentryCache.isNull())
	{
		QList<FuseDirEntryBuf *> sizeMap;
		size_t total_size = 0;

		sizeMap.append(new FuseDirEntryBuf(this, "."));

		Folder *parent = dynamic_cast<Folder *>(this->parent()) ?: this;
		sizeMap.append(new FuseDirEntryBuf(parent, ".."));

		foreach (auto item, _contents)
		{
			sizeMap.append(new FuseDirEntryBuf(item));
		}

		foreach (auto item, sizeMap){
			total_size += item->size;
		}

		char *buf = new char[total_size]();
		off_t offset = 0;

		for(auto i = 0; i < sizeMap.size() - 1; i++)
		{
			auto item = sizeMap.at(i);

			fuse_add_direntry(0, buf + offset, total_size - offset, item->name, &item->stat, offset + item->size);
			offset += item->size;
		}

		auto item = sizeMap.last();
		fuse_add_direntry(0, buf + offset, total_size - offset, item->name, &item->stat, -1);

		while (!sizeMap.isEmpty())
			delete sizeMap.takeFirst();

		_fuseDirentryCache = QByteArray(buf, total_size);
		delete[] buf;
	}

	return _fuseDirentryCache;
}


void Folder::wfuse_apply_stat(struct stat *st)
{
	st->st_mode = S_IFDIR | (S_IRUSR | S_IXUSR | S_IRGRP | S_IXGRP | S_IROTH | S_IXOTH);
}
