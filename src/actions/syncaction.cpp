#include "syncaction.h"

SyncAction::SyncAction(fuse_req_t req, Controller *controller)
	:_req(req), _isFinished(false), _controller(controller)
{
	if(!controller)
	{
		finishWithError(EFAULT);
		throw "Could not create action, because controller is NULL!";
	}
}

int SyncAction::finishWithError(int error)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_err(_req, error);
}

int SyncAction::finishWithAttr(const Stat *attr, double attr_timeout)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_attr(_req, attr, attr_timeout);
}

int SyncAction::finishWithEntry(const fuse_entry_param *e)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_entry(_req, e);
}

int SyncAction::finishWithOpen(const fuse_file_info *fi)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_open(_req, fi);
}

int SyncAction::finishWithBuffer(const char *buf, size_t size)
{
	if(_isFinished)
		return -1;

	_isFinished = true;
	deleteLater();
	return fuse_reply_buf(_req, buf, size);
}

int SyncAction::finishWithBuffer(QByteArray data)
{
	if(data.isEmpty())
		return finishWithBuffer(0, 0);

	return finishWithBuffer(data.constData(), data.size());
}
