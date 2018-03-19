#ifndef CALLBACKWITHARGUMENT_H
#define CALLBACKWITHARGUMENT_H

#include <functional>
#include "icallback.h"

template <class T, class TArg>
class CallbackWithArgument : public ICallbackWithArgument<TArg>
{
public:
	CallbackWithArgument(T* obj, std::function<void(T&, TArg)> success, std::function<void(T&, QString)> error)
		:_obj(obj), _success(success), _error(error), _errorCallback(nullptr)
	{

	}

	CallbackWithArgument(T* obj, std::function<void(T&, TArg)> success, ICallbackBase *error)
		:_obj(obj), _success(success), _error(nullptr), _errorCallback(error)
	{

	}

	virtual ~CallbackWithArgument()
	{
		_obj = 0;
		_error = 0;
		_errorCallback = nullptr;
		_success = nullptr;
		_error = nullptr;
	}

	void success(TArg arg) override
	{
		if(_obj)
		{
			_success(*_obj, arg);
			delete this;
		}
	}

	void error(QString msg) override
	{
		if(_obj)
		{
			if(_errorCallback)
				_errorCallback->error(msg);
			else
				_error(*_obj, msg);

			delete this;
		}
	}

private:
	T *_obj;
	std::function<void(T&, TArg)> _success;
	std::function<void(T&, QString)> _error;
	ICallbackBase *_errorCallback;
};



#endif // CALLBACKWITHARGUMENT_H
