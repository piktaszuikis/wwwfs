#ifndef ISIZECALLBACK_H
#define ISIZECALLBACK_H

#include <stddef.h>
#include <QString>

class IFileInfoCallback
{
public:
	virtual void setInfo(size_t size, QString mime) = 0;
	virtual void setError(QString error) = 0;
};

#endif // ISIZECALLBACK_H
