#ifndef CONTENTCACHE_H
#define CONTENTCACHE_H

#include <QUrl>
#include <QHash>

#include "cache/cacheditem.h"

class ContentCache
{
public:
	ContentCache();

	bool isContentCached(QUrl url);
	bool isContentCached(QUrl url, off_t offset, size_t size);

	bool isResourceInfoCached(QUrl url);

	QByteArray getContent(QUrl url);
	QByteArray getContent(QUrl url, off_t offset, size_t size);

	RemoteResourceInfo *getResourceInfo(QUrl url);

	void cacheContent(QUrl url, off_t offset, QByteArray content);
	void cacheResourceInfo(QUrl url, RemoteResourceInfo *info);

private:
	void cleanup();
	long cachedSize();
	long removeOldest();
	void removeSize(long howMuchToRemove);

	QHash<QUrl, CachedItem *> _cache;

};

#endif // CONTENTCACHE_H
