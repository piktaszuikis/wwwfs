#ifndef CALLBACKWITHARGUMENTANDDATA_H
#define CALLBACKWITHARGUMENTANDDATA_H

#include <functional>
#include "icallback.h"

template <class T, class TArg, class TData>
class CallbackWithArgumentAndData : public ICallbackWithArgument<TArg>
{
public:
	CallbackWithArgumentAndData(T* obj, TData data, std::function<void(T&, TArg, TData)> success, std::function<void(T&, QString, TData)> error)
		:_obj(obj), _data(data), _success(success), _error(error), _errorCallback(nullptr)
	{

	}

	CallbackWithArgumentAndData(T* obj, TData data, std::function<void(T&, TArg)> success, ICallbackBase *error)
		:_obj(obj), _data(data), _success(success), _error(nullptr), _errorCallback(error)
	{

	}

	virtual ~CallbackWithArgumentAndData()
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
			_success(*_obj, arg, _data);
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
				_error(*_obj, msg, _data);

			delete this;
		}
	}

private:
	T *_obj;
	TData _data;
	std::function<void(T&, TArg, TData)> _success;
	std::function<void(T&, QString, TData)> _error;
	ICallbackBase *_errorCallback;
};

#endif // CALLBACKWITHARGUMENTANDDATA_H
