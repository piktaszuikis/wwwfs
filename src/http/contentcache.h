#ifndef CONTENTCACHE_H
#define CONTENTCACHE_H

#include <QUrl>
#include <QHash>

class ContentCache
{
public:
	ContentCache();

	bool isRangeSupportedCached(QUrl url);
	bool isRangeSupported(QUrl url);
	bool isSizeCached(QUrl url);
	bool isMimeCached(QUrl url);
	bool isContentCached(QUrl url);
	bool isContentCached(QUrl url, off_t offset, size_t size);

	size_t getSize(QUrl url);
	QString getMime(QUrl url);
	QByteArray getContent(QUrl url);
	QByteArray getContent(QUrl url, off_t offset, size_t size);

	void cacheSize(QUrl url, size_t size);
	void cacheMime(QUrl url, QString mime);
	void cacheIsRangeSupported(QUrl url, bool isSupported);
	void cacheContent(QUrl url, off_t offset, QByteArray array);

private:
	QUrl getUrl(QUrl &url);

	QHash<QUrl, size_t> _sizeCache;
	QHash<QUrl, QString> _mimeCache;
	QHash<QUrl, QByteArray> _contentCache;

};

#endif // CONTENTCACHE_H
