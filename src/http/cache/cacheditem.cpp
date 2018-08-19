#include "cacheditem.h"

CachedItem::CachedItem()
	: _info(nullptr)
{
	_lastAccess = QDateTime::currentDateTime();
}

CachedItem::~CachedItem()
{
	_lastAccess = QDateTime();

	while(!_data.isEmpty())
		delete _data.takeLast();
}

QSharedPointer<RemoteResourceInfo> CachedItem::info()
{
	setAccessed();
	return _info;
}

bool CachedItem::isDataCachedFully()
{
	if(info() && info()->size == usedMemorySize())
		return true;

	return false;
}

bool CachedItem::isDataCached(off_t offset, size_t size)
{
	if(_data.isEmpty())
		return false;

	if(info())
	{
		if(static_cast<long long>(offset) > static_cast<long long>(info()->size))
			return true;

		if(offset + static_cast<long>(size) > static_cast<long>(info()->size))
			size = static_cast<size_t>(static_cast<long>(info()->size) - offset);
	}

	int i = 0;
	long end = offset + static_cast<long>(size);
	//find first intersection
	for(; i < _data.count(); i++)
	{
		if(_data[i]->end() > offset)
			break;
	}

	if(i >= _data.count() || offset < _data[i]->offset())
		return false;

	if(_data[i]->end() >= end)
		return true;

	while(++i < _data.count())
	{
		if(_data[i - 1]->end() != _data[i]->offset())
			return false;

		if(_data[i]->end() >= end)
			return true;
	}

	return false;
}

QByteArray CachedItem::dataFull()
{
	QByteArray result;

	if(isDataCachedFully())
	{
		setAccessed();

		if(_data.count() == 1)
		{
			_data.first()->setAccessed();
			return _data.first()->data();
		}

		foreach (CachedPiece *item, _data)
		{
			item->setAccessed();
			result.append(item->data());
		}
	}

	return result;
}

QByteArray CachedItem::data(off_t offset, size_t size)
{
	QByteArray result;
	setAccessed();

	if(info())
	{
		if(static_cast<long>(offset) > static_cast<long>(info()->size))
			return result;

		if(offset + static_cast<long>(size) > static_cast<long>(info()->size))
			size = static_cast<size_t>(static_cast<long>(info()->size) - offset);
	}

	int i = 0;
	long end = offset + static_cast<long>(size);
	//find first intersection
	for(; i < _data.count(); i++)
	{
		if(_data[i]->end() > offset)
			break;
	}

	if(i >= _data.count() || end < _data[i]->offset())
		return result;

	while(i < _data.count() && _data[i]->offset() < end)
	{
		auto data = _data[i++];

		long start = offset > data->offset() ? offset - data->offset() : 0L;
		long len = data->end() < end ? -1 : end - qMax(offset, data->offset());

		data->setAccessed();

		if(start == 0 && len == -1)
			result.append(data->data());
		else
			result.append(data->data().mid(static_cast<int>(start), static_cast<int>(len)));
	}

	return result;
}

void CachedItem::cacheData(off_t offset, QByteArray data)
{
	if(offset == 0 && info() && info()->size == static_cast<size_t>(data.size()))
	{
		while(!_data.isEmpty())
			delete _data.takeLast();

		_data.append(new CachedPiece(offset, data));
	}
	else
	{
		int index = 0;
		int i = 0;

		for(; i < _data.count() && index < data.count(); i++)
		{
			auto item = _data[i];

			if(item->end() < offset)
				continue;

			long len = item->offset() - offset;
			if(len > 0)
			{
				if(len >= data.size())
				{
					_data.insert(i++, new CachedPiece(offset, data));
					index += len;
					break;
				}
				else
				{
					_data.insert(i++, new CachedPiece(offset, data.mid(index, static_cast<int>(len))));
				}
			}

			index += item->end() - offset;
			offset = item->end();
		}

		if(index < data.count())
			_data.insert(i, new CachedPiece(offset, data.mid(index, -1)));
	}
}

size_t CachedItem::usedMemorySize()
{
	size_t size = 0;

	if(_data.count() > 0)
	{
		foreach (auto item, _data)
		{
			size += static_cast<size_t>(item->data().size());
		}
	}

	return size;
}

long CachedItem::remove(long memorySizeToRemove)
{
	long freedMemory = 0;

	while(memorySizeToRemove > 0 && _data.count() > 0)
	{
		QDateTime timestamp = QDateTime::currentDateTime();
		int removeAt = -1;

		for (auto i = _data.count() - 1; i >= 0; i--)
		{
			if(_data[i]->lastAccess() <= timestamp)
			{
				timestamp = _data[i]->lastAccess();
				removeAt = i;
			}
		}

		if(removeAt >= 0)
		{
			CachedPiece *piece = _data.takeAt(removeAt);
			freedMemory += piece->data().size();
			memorySizeToRemove -= piece->data().size();
			delete piece;
		}
		else
		{
			break;
		}
	}

	return freedMemory;
}
