#include "cachedpiece.h"

CachedPiece::CachedPiece(off_t offset, QByteArray data)
	: _offset(offset), _data(data)
{
	_lastAccess = QDateTime::currentDateTime();
}

CachedPiece::~CachedPiece()
{
	_offset = 0;
	_lastAccess = QDateTime();
	_data.clear();
}
