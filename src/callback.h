#ifndef CALLBACK_H
#define CALLBACK_H

#include <functional>
#include "icallback.h"

template <class T>
class Callback : public ICallback
{
public:
	Callback(T* obj, std::function<void(T&)> success, std::function<void(T&, QString)> error)
		:_obj(obj), _success(success), _error(error)
	{

	}

	virtual ~Callback()
	{
		_obj = 0;
		_success = nullptr;
		_error = nullptr;
	}

	void success() override
	{
		if(_obj)
		{
			_success(*_obj);
			delete this;
		}
	}

	void error(QString msg) override
	{
		if(_obj)
		{
			_error(*_obj, msg);
			delete this;
		}
	}

private:
	T *_obj;
	std::function<void(T&)> _success;
	std::function<void(T&, QString)> _error;
};

#endif // CALLBACK_H
