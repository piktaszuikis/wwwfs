#ifndef ICALLBACK_H
#define ICALLBACK_H

#include <QString>

class ICallbackBase
{
public:
	virtual ~ICallbackBase() {}
	virtual void error(QString) = 0;
};

template <class T> class ICallbackWithArgument : public ICallbackBase
{
public:
	virtual void success(T) = 0;
};

class ICallback : public ICallbackBase
{
public:
	virtual void success() = 0;
};

#endif // ICALLBACK_H
