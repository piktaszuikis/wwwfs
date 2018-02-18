#include "contentcache.h"

ContentCache::ContentCache()
{

}

bool ContentCache::isRangeSupportedCached(QUrl url)
{
	return true;
}

bool ContentCache::isRangeSupported(QUrl url)
{
	return false;
}

bool ContentCache::isSizeCached(QUrl url)
{
	return _sizeCache.contains(getUrl(url));
}

bool ContentCache::isMimeCached(QUrl url)
{
	return _mimeCache.contains(getUrl(url));
}

bool ContentCache::isContentCached(QUrl url)
{
	return _contentCache.contains(getUrl(url));
}

bool ContentCache::isContentCached(QUrl url, off_t offset, size_t size)
{
	return _contentCache.contains(getUrl(url));
}

size_t ContentCache::getSize(QUrl url)
{
	return _sizeCache.value(getUrl(url));
}

QString ContentCache::getMime(QUrl url)
{
	return _mimeCache.value(getUrl(url));
}

QByteArray ContentCache::getContent(QUrl url)
{
	return _contentCache.value(getUrl(url));
}

QByteArray ContentCache::getContent(QUrl url, off_t offset, size_t size)
{
	return _contentCache.value(getUrl(url)).mid(offset, size);
}

void ContentCache::cacheSize(QUrl url, size_t size)
{
	_sizeCache[getUrl(url)] = size;
}

void ContentCache::cacheMime(QUrl url, QString mime)
{
	_mimeCache[getUrl(url)] = mime;
}

void ContentCache::cacheIsRangeSupported(QUrl url, bool isSupported)
{
	;
}

void ContentCache::cacheContent(QUrl url, off_t offset, QByteArray array)
{
	_contentCache[getUrl(url)] = array;
}

QUrl ContentCache::getUrl(QUrl &url)
{
	return url.adjusted(QUrl::RemoveFragment);
}
