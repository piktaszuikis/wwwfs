#include "httpclient.h"
#include "../actions/readdiraction.h"
#include "httpitemcollectionloader.h"
#include <QNetworkReply>

#define PROP_OFFSET "request_offset"
#define PROP_SIZE "request_size"
#define PROP_CALLBACK "request_callback"

HttpClient::HttpClient(QObject *parent)
	: QObject(parent), _manager(0)
{
	_manager = new QNetworkAccessManager(this);
	_cache = new ContentCache();

	connect(this, &HttpClient::getContentInfo, this, &HttpClient::onGetContentInfo);
	connect(this, SIGNAL(getContent(QUrl,IContentCallback*)), this, SLOT(onGetContent(QUrl,IContentCallback*)));
	connect(this, SIGNAL(getContent2(QUrl,long long,long long,IContentCallback*)), this, SLOT(onGetContent2(QUrl,long long,long long,IContentCallback*)));
}

void HttpClient::onGetContent(QUrl url, IContentCallback *callback)
{
	if(callback)
	{
		if(_cache->isContentCached(url))
		{
			callback->setContent(url, _cache->getContent(url));
		}
		else
		{
			QNetworkRequest request(url);
			request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

			QNetworkReply *reply = _manager->get(request);
			reply->setProperty(PROP_CALLBACK, (qlonglong)callback);

			connect(reply, &QNetworkReply::finished, this, &HttpClient::handleContentReply);
		}
	}
}

void HttpClient::onGetContent2(QUrl url, long long offset, long long size, IContentCallback *callback)
{
	if(callback)
	{
		if(offset < 0)
		{
			callback->setError("offset must be >= 0");
		}
		else if(size <= 0)
		{
			callback->setContent(url, QByteArray());

		}
		else if(_cache->isContentCached(url, offset, size))
		{
			callback->setContent(url, _cache->getContent(url, offset, size));
		}
		else
		{
			QNetworkRequest request(url);
			request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

			if(_cache->isRangeSupported(url))
				request.setRawHeader("Range", QString("bytes=%1-%2").arg(offset).arg(size).toUtf8());

			QNetworkReply *reply = _manager->get(request);
			reply->setProperty(PROP_OFFSET, (qlonglong)offset);
			reply->setProperty(PROP_SIZE, (qlonglong)size);
			reply->setProperty(PROP_CALLBACK, (qlonglong)callback);

			//connect(reply, &QNetworkReply::readyRead, this, &HttpClient::handlePartialContentReply);
			connect(reply, &QNetworkReply::finished, this, &HttpClient::handlePartialContentReply);
		}
	}
}

void HttpClient::onGetContentInfo(QUrl url, IFileInfoCallback *callback)
{
	if(callback)
	{
		if(_cache->isSizeCached(url) && _cache->isMimeCached(url))
		{
			callback->setInfo(_cache->getSize(url), _cache->getMime(url));
		}
		else
		{
			QNetworkRequest request(url);
			request.setAttribute(QNetworkRequest::FollowRedirectsAttribute, true);

			QNetworkReply *reply = _manager->head(request);
			reply->setProperty(PROP_CALLBACK, (qlonglong)callback);

			connect(reply, &QNetworkReply::finished, this, &HttpClient::handleInfoReply);
		}
	}
}

void HttpClient::handleContentReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	IContentCallback *callback = this->callback<IContentCallback>(reply);

	if(reply)
	{
		if(callback)
		{
			if(reply->error() == QNetworkReply::NoError)
			{
				QUrl url = reply->request().url();
				QByteArray data = reply->readAll();

				_cache->cacheContent(url, 0, data);
				callback->setContent(url, data);
			}
			else
			{
				callback->setError(reply->errorString());
			}
		}

		reply->deleteLater();
	}
}

void HttpClient::handlePartialContentReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	IContentCallback *callback = this->callback<IContentCallback>(reply);
	size_t size = this->size(reply);
	off_t offset = this->offset(reply);

	if(reply)
	{
		QUrl url = reply->request().url();

		if(!reply->isFinished())
		{
			if(_cache->isRangeSupported(url))
			{
				if(reply->size() < size)
					return; //let's wait for more data
			}
			else
			{
				if(reply->size() < size + offset)
					return; //let's wait for more data
			}
		}

		if(callback)
		{
			if(reply->error() == QNetworkReply::NoError)
			{
				QByteArray data = reply->readAll();

				if(_cache->isRangeSupported(url))
				{
					_cache->cacheContent(url, offset, data);
					callback->setContent(url, data);
				}
				else
				{
					_cache->cacheContent(url, 0, data);

					callback->setContent(url, data.mid(offset, size));
				}
			}
			else
			{
				callback->setError(reply->errorString());
			}
		}

		reply->deleteLater();
	}
}

void HttpClient::handleInfoReply()
{
	QNetworkReply *reply = qobject_cast<QNetworkReply *>(sender());
	IFileInfoCallback *callback = this->callback<IFileInfoCallback>(reply);

	if(reply)
	{
		if(callback)
		{
			if(reply->error() == QNetworkReply::NoError)
			{
				auto length = reply->header(QNetworkRequest::ContentLengthHeader);

				if(length.isValid())
				{
					QUrl url = reply->request().url();
					QString mime = this->mime(reply);
					_cache->cacheSize(url, length.value<size_t>());
					_cache->cacheMime(url, mime);
					_cache->cacheIsRangeSupported(url, isRangeSupported(reply));

					callback->setInfo(length.value<size_t>(), mime);
				}
				else
					callback->setError("No content length header");
			}
			else
			{
				callback->setError(reply->errorString());
			}
		}

		reply->deleteLater();
	}
}

bool HttpClient::isRangeSupported(QNetworkReply *reply)
{
	if(reply->hasRawHeader("Content-Range"))
		return true;

	if(reply->hasRawHeader("Accept-Ranges"))
		return reply->rawHeader("Accept-Ranges") == "bytes";

	return false;
}

QString HttpClient::mime(QNetworkReply *reply)
{
	QString mime = reply->header(QNetworkRequest::ContentTypeHeader).toString();

	int index = mime.indexOf((';')); //remove encoding
	if(index >= 0)
		mime = mime.mid(0, index);

	return mime;
}

size_t HttpClient::size(QNetworkReply *reply)
{
	if(reply)
	{
		bool ok = false;
		size_t result = (size_t)reply->property(PROP_SIZE).toLongLong(&ok);

		if(ok)
			return result;
	}

	return 0;
}

off_t HttpClient::offset(QNetworkReply *reply)
{
	if(reply)
	{
		bool ok = false;
		off_t result = (off_t)reply->property(PROP_OFFSET).toLongLong(&ok);

		if(ok)
			return result;
	}

	return 0;
}

template<typename T> T *HttpClient::callback(QNetworkReply *reply)
{
	if(reply)
	{
		bool ok = false;
		T* result = static_cast<T *>((void*)reply->property(PROP_CALLBACK).toLongLong(&ok));

		if(ok)
			return result;
	}

	return nullptr;
}
