#ifndef CACHEDPIECE_H
#define CACHEDPIECE_H

#include <QDateTime>
#include <QByteArray>

class CachedPiece
{
public:
	CachedPiece(off_t offset, QByteArray data);
	virtual ~CachedPiece();

	inline QByteArray data() const { return _data; }
	inline off_t offset() const { return _offset; }
	inline off_t end() const { return _offset + _data.count(); }
	inline QDateTime lastAccess() const { return _lastAccess; }

	inline void setAccessed(){ _lastAccess = QDateTime::currentDateTime(); }

private:
	off_t _offset;
	QByteArray _data;
	QDateTime _lastAccess;
};

#endif // CACHEDPIECE_H
