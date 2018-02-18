#ifndef ICONTENTCALLBACK_H
#define ICONTENTCALLBACK_H

#include <QByteArray>

class IContentCallback
{
public:
	virtual void setContent(QByteArray content) = 0;
	virtual void setError(QString error) = 0;
};

#endif // ICONTENTCALLBACK_H
