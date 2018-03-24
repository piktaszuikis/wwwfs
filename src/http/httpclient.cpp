#include "httpclient.h"
#include "configurationmanager.h"
#include <QNetworkReply>

#define PROP_REPLY_DATA "reply_data"

QString extractMime(QNetworkReply *reply)
{
	QString mime = reply->header(QNetworkRequest::ContentTypeHeader).toString();

	int index = mime.indexOf((';')); //remove encoding
	if(index >= 0)
		mime = mime.mid(0, index);

	return mime;
}

bool extractIsRangeSupported(QNetworkReply *reply)
{
	if(reply->hasRawHeader("Content-Range"))
		return true;

	if(reply->hasRawHeader("Accept-Ranges"))
		return reply->rawHeader("Accept-Ranges") == "bytes";

	return false;
}

QByteArray *getFullReplyData(QNetworkReply *reply)
{
	if(reply)
	{
		bool ok = false;
		QByteArray *result = static_cast<QByteArray *>((void*)reply->property(PROP_REPLY_DATA).toLongLong(&ok));

		if(ok)
			return result;

		result = new QByteArray();
		reply->setProperty(PROP_REPLY_DATA, (qlonglong)result);
		reply->connect(reply, &QObject::destroyed, [result]
		{
			//reply->setProperty(PROP_REPLY_DATA, QVariant());
			result->clear();
			delete result;
		});

		return result;
	}

	return nullptr;
}

QNetworkRequest createRequest(QUrl &url){

	//qDebug() << "Making a request: " << url.toString(QUrl::None);
	QNetworkRequest request(url);
	request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

	return request;
}

//---------------

HttpClient::HttpClient(QObject *parent)
	: QObject(parent), _manager(0)
{
	_manager = new QNetworkAccessManager(this);
	_cache = new ContentCache();
}

void HttpClient::get(QUrl url, ICallbackWithArgument<QByteArray> *callback)
{
	if(!url.isValid())
		throw "ArgumentNullException: url";

	if(!callback)
		throw "ArgumentNullException: callback";

	if(_cache->isContentCached(url))
	{
		callback->success(_cache->getContent(url));
		return;
	}

	QNetworkRequest request = createRequest(url);
	qDebug() << "[GET]" << url.toString(QUrl::None);

	QNetworkReply *reply = _manager->get(request);

	reply->connect(reply, &QNetworkReply::finished, [this, url, callback, reply]
	{
		if(reply->error() == QNetworkReply::NoError)
		{
			QByteArray data = reply->readAll();
			_cache->cacheContent(url, 0, data);

			callback->success(data);
		}
		else
			callback->error(reply->errorString());

		reply->deleteLater();
	});
}

void HttpClient::get(QUrl url, qlonglong offset, qlonglong size, ICallbackWithArgument<QByteArray> *callback)
{
	if(!url.isValid())
		throw "ArgumentNullException: url";

	if(!callback)
		throw "ArgumentNullException: callback";

	if(offset < 0)
		throw "ArgumentInvalidException: offset must be >= 0";

	if(size < 0)
		throw "ArgumentInvalidException: size must be >= 0";

	if(_cache->isContentCached(url, offset, size))
	{
		callback->success(_cache->getContent(url, offset, size));
		return;
	}

	QNetworkRequest request = createRequest(url);
	qDebug() << QString("[GET Range: %1-%2]").arg(offset).arg(offset + size) << url.toString(QUrl::None);
	request.setRawHeader("Range", QString("bytes=%1-%2").arg(offset).arg(offset + size).toUtf8());

	QNetworkReply *reply = _manager->get(request);
	reply->setReadBufferSize(ConfigurationManager::sampleSize());

	reply->connect(reply, &QNetworkReply::readyRead, [this, url, offset, size, callback, reply]
	{
		getContent(url, offset, size, callback, reply);
	});

	reply->connect(reply, &QNetworkReply::finished, [this, url, offset, size, callback, reply]
	{
		getContent(url, offset, size, callback, reply);
	});

}

void HttpClient::getInfo(QUrl url, ICallbackWithArgument<QSharedPointer<RemoteResourceInfo> > *callback)
{
	if(!url.isValid())
		throw "ArgumentNullException: url";

	if(!callback)
		throw "ArgumentNullException: callback";

	if(_cache->isResourceInfoCached(url))
	{
		callback->success(_cache->getResourceInfo(url));
		return;
	}

	QNetworkRequest request = createRequest(url);
	qDebug() << "[HEAD]" << url.toString(QUrl::None);

	QNetworkReply *reply = _manager->head(request);
	reply->connect(reply, &QNetworkReply::finished, [this, reply, callback, url]
	{
		if(reply->error() == QNetworkReply::NoError)
		{
			auto length = reply->header(QNetworkRequest::ContentLengthHeader);
			auto mime = extractMime(reply);

			if(length.isValid() && !mime.isNull())
			{
				QSharedPointer<RemoteResourceInfo> info(new RemoteResourceInfo(mime, length.value<size_t>()));

				callback->success(info);

				_cache->cacheResourceInfo(url, info);
			}
			else
			{
				getInfoFromContent(url, callback);
				return;
			}
		}
		else
		{
			callback->error(reply->errorString());
		}

		reply->deleteLater();
	});
}

//---------------

void HttpClient::getContent(QUrl url, qlonglong offset, qlonglong size, ICallbackWithArgument<QByteArray> *callback, QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
	{
		if(reply->error() != QNetworkReply::OperationCanceledError)
			callback->error(reply->errorString());

		reply->deleteLater();
		return;
	}

	qlonglong expectedSize = size;
	bool isRangeSupported = extractIsRangeSupported(reply);

	if(!isRangeSupported)
		expectedSize += offset;

	QByteArray *fullData = getFullReplyData(reply);
	fullData->append(reply->readAll());

	if((fullData->size() >= expectedSize && !isRangeSupported) || reply->isFinished())
	{
		_cache->cacheContent(url, isRangeSupported ? offset : 0, *fullData);
		callback->success(fullData->mid(isRangeSupported ? 0 : offset, size));

		if(!isRangeSupported)
			reply->abort();

		reply->deleteLater();
	}

}

void HttpClient::getInfoFromContent(QUrl url, ICallbackWithArgument<QSharedPointer<RemoteResourceInfo> > *callback)
{
	QNetworkRequest request = createRequest(url);
	qDebug() << "[GET]" << url.toString(QUrl::None);
	QNetworkReply *reply = _manager->get(request);

	reply->connect(reply, &QNetworkReply::finished, [this, url, callback, reply]
	{
		getInfoFromContent(url, callback, reply);
	});

}

void HttpClient::getInfoFromContent(QUrl url, ICallbackWithArgument<QSharedPointer<RemoteResourceInfo> > *callback, QNetworkReply *reply)
{
	if(reply->error() != QNetworkReply::NoError)
	{
		callback->error(reply->errorString());
		reply->deleteLater();
		return;
	}

	auto mime = extractMime(reply);

	QByteArray data = reply->readAll();
	_cache->cacheContent(url, 0, data);
	int length = data.length();

	if(!mime.isNull())
	{
		QSharedPointer<RemoteResourceInfo> info(new RemoteResourceInfo(mime, length));

		callback->success(info);

		_cache->cacheResourceInfo(url, info);
		reply->deleteLater();
	}
	else
	{
		reply->abort();
		reply->deleteLater();
		callback->error(QString("Server did not return mime (%1) for url: %2").arg(mime).arg(url.toString()));
	}


}

