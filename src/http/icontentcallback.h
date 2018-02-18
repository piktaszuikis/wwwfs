#ifndef ICONTENTCALLBACK_H
#define ICONTENTCALLBACK_H

#include <QByteArray>
#include <QUrl>

class IContentCallback
{
public:
	virtual void setContent(QUrl url, QByteArray content) = 0;
	virtual void setError(QString error) = 0;
};

#endif // ICONTENTCALLBACK_H
