#include "action.h"

Action::Action(fuse_req_t req, Controller *controller)
	:_req(req), _isFinished(false), _asyncStarted(false), _controller(controller)
{
	if(!controller)
	{
		finishWithError(EFAULT);
		throw "Could not create action, because controller is NULL!";
	}
}

Action::~Action()
{
	_isFinished = true;
	_controller= nullptr;
	_req = nullptr;
}

int Action::finishWithError(int error)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_err(_req, error);
}

int Action::finishWithAttr(const Stat *attr, double attr_timeout)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_attr(_req, attr, attr_timeout);
}

int Action::finishWithEntry(const fuse_entry_param *e)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_entry(_req, e);
}

int Action::finishWithOpen(const fuse_file_info *fi)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_open(_req, fi);
}

int Action::finishWithBuffer(const char *buf, size_t size)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_buf(_req, buf, size);
}

int Action::finishWithBuffer(QByteArray data)
{
	if(data.isEmpty())
		return finishWithBuffer(nullptr, 0);

	return finishWithBuffer(data.constData(), static_cast<size_t>(data.size()));
}

void Action::startAsync()
{
	if(_asyncStarted)
		return;
	_asyncStarted = true;

	moveToThread(controller());
	connect(this, &Action::_doStartAsync, this, &Action::doRunAsync, Qt::QueuedConnection);
	emit _doStartAsync();
}

void Action::doRunAsync()
{
	disconnect(this, &Action::_doStartAsync, this, &Action::doRunAsync);
	asyncAction();
}

void Action::asyncAction()
{

}

