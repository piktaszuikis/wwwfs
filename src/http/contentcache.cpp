#include "contentcache.h"
#include "configurationmanager.h"

#include <QDebug>

ContentCache::ContentCache()
{

}


bool ContentCache::isContentCached(QUrl url)
{
	return _cache.contains(url) && _cache[url]->isDataCachedFully();
}

bool ContentCache::isContentCached(QUrl url, off_t offset, size_t size)
{
	return _cache.contains(url) && _cache[url]->isDataCached(offset, size);
}

bool ContentCache::isResourceInfoCached(QUrl url)
{
	return _cache.contains(url) && _cache[url]->info();
}

QByteArray ContentCache::getContent(QUrl url)
{
	if(_cache.contains(url))
		return _cache[url]->dataFull();

	return QByteArray();
}

QByteArray ContentCache::getContent(QUrl url, off_t offset, size_t size)
{
	if(_cache.contains(url))
		return _cache[url]->data(offset, size);

	return QByteArray();
}

QSharedPointer<RemoteResourceInfo> ContentCache::getResourceInfo(QUrl url)
{
	if(_cache.contains(url))
		return _cache[url]->info();

	return nullptr;
}

void ContentCache::cacheContent(QUrl url, off_t offset, QByteArray content)
{
	if(ConfigurationManager::cacheRamLength() == 0)
		return;

	if(content.size() >= ConfigurationManager::cacheRamSize())
		return;

	if(!_cache.contains(url))
		_cache[url] = new CachedItem();

	_cache[url]->cacheData(offset, content);
	cleanup();
}

void ContentCache::cacheResourceInfo(QUrl url, QSharedPointer<RemoteResourceInfo> info)
{
	if(ConfigurationManager::cacheRamLength() == 0)
		return;

	if(!_cache.contains(url))
		_cache[url] = new CachedItem();

	_cache[url]->setInfo(info);

	cleanup();
}

void ContentCache::cleanup()
{
	while(_cache.count() > ConfigurationManager::cacheRamLength())
	{
		if(removeOldest() <= 0)
			break;;
	}

	long usedMemory = cachedSize();
	if(usedMemory > ConfigurationManager::cacheRamSize())
	{
		removeSize(usedMemory - ConfigurationManager::cacheRamSize());
	}
}

long ContentCache::cachedSize()
{
	long size = 0;

	foreach(CachedItem *item, _cache.values())
	{
		size += item->usedMemorySize();
	}

	return size;
}

long ContentCache::removeOldest()
{
	long freedMemorySize = 0;
	QUrl key;
	QDateTime timestamp = QDateTime::currentDateTime();

	auto i = _cache.constBegin();
	while(i != _cache.constEnd())
	{
		if(i.value()->lastAccess() <= timestamp)
		{
			key = i.key();
			timestamp = i.value()->lastAccess();
		}

		++i;
	}

	if(_cache.contains(key))
	{
		qDebug() << "[UNCACHE full]" << key.toString(QUrl::None);

		auto item = _cache.take(key);
		freedMemorySize = item->usedMemorySize();
		delete item;
	}

	return freedMemorySize;
}

void ContentCache::removeSize(long howMuchToRemove)
{
	while(howMuchToRemove > 0)
	{
		QUrl key;
		QDateTime timestamp = QDateTime::currentDateTime();

		auto i = _cache.constBegin();
		while(i != _cache.constEnd())
		{
			if(i.value()->lastAccess() <= timestamp && i.value()->usedMemorySize() > 0)
			{
				key = i.key();
				timestamp = i.value()->lastAccess();
			}

			++i;
		}

		if(_cache.contains(key))
		{
			auto toRemove = howMuchToRemove;
			howMuchToRemove -= _cache[key]->remove(howMuchToRemove);
			qDebug() << QString("[UNCACHE %1/%2]").arg(toRemove).arg(howMuchToRemove) << key.toString(QUrl::None);
		}
		else
		{
			break;
		}
	}
}

