#ifndef HTTPCLIENT_H
#define HTTPCLIENT_H

#include <QNetworkAccessManager>

#include "contentcache.h"
#include "icontentcallback.h"
#include "ifileinfocallback.h"

class HttpClient : public QObject
{
	Q_OBJECT
public:
	explicit HttpClient(QObject *parent = nullptr);

signals:
	void getContent(QUrl url, IContentCallback *callback);
	void getContent2(QUrl url, long long offset, long long size, IContentCallback *callback);
	void getContentInfo(QUrl url, IFileInfoCallback *callback);

private slots:
	void onGetContent(QUrl url, IContentCallback *callback);
	void onGetContent2(QUrl url, long long offset, long long size, IContentCallback *callback);
	void onGetContentInfo(QUrl url, IFileInfoCallback *callback);

	void handleContentReply();
	void handlePartialContentReply();
	void handleInfoReply();

private:
	bool isRangeSupported(QNetworkReply *reply);
	QString mime(QNetworkReply *reply);

	template<typename T> T *callback(QNetworkReply *reply);
	size_t size(QNetworkReply *reply);
	off_t offset(QNetworkReply *reply);

	QNetworkAccessManager *_manager;
	ContentCache *_cache;
};

#endif // HTTPCLIENT_H
