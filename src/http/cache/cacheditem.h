#ifndef CACHEDITEM_H
#define CACHEDITEM_H

#include <QDateTime>
#include "../remoteresourceinfo.h"
#include "cachedpiece.h"

class CachedItem
{
public:
	CachedItem();
	virtual ~CachedItem();

	inline void cacheInfo(RemoteResourceInfo *info) { _info = info; }
	RemoteResourceInfo *info();

	bool isDataCachedFully();
	bool isDataCached(off_t offset, size_t size);

	QByteArray dataFull();
	QByteArray data(off_t offset, size_t size);

	void cacheData(off_t offset, QByteArray data);
	size_t usedMemorySize();

	long remove(long memorySizeToRemove);
	inline QDateTime lastAccess() const { return _lastAccess; }

private:
	inline void setAccessed(){ _lastAccess = QDateTime::currentDateTime(); }

	QDateTime _lastAccess;
	RemoteResourceInfo *_info;
	QList<CachedPiece *> _data;

};

#endif // CACHEDITEM_H
