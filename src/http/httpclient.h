#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QNetworkAccessManager>

#include "icallback.h"
#include "contentcache.h"
#include "icontentcallback.h"
#include "remoteresourceinfo.h"

class HttpClient : public QObject
{
	Q_OBJECT
public:
	explicit HttpClient(QObject *parent = nullptr);

	void get(QUrl url, ICallbackWithArgument<QByteArray> *callback);
	void get(QUrl url, qlonglong offset, qlonglong size, ICallbackWithArgument<QByteArray> *callback);
	void getInfo(QUrl url, ICallbackWithArgument<RemoteResourceInfo *> *callback);

private:
	void getContent(QUrl url, qlonglong offset, qlonglong size, ICallbackWithArgument<QByteArray> *callback, QNetworkReply *reply);
	void getInfoFromContent(QUrl url, ICallbackWithArgument<RemoteResourceInfo *> *callback);
	void getInfoFromContent(QUrl url, ICallbackWithArgument<RemoteResourceInfo *> *callback, QNetworkReply *reply);

private:
	QNetworkAccessManager *_manager;
	ContentCache *_cache;
};

#endif // HTTPCLIENT_H
