#include "readaction.h"
#include "callbackwithargument.h"
#include <QDebug>

ReadAction::ReadAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi, Controller *controller)
	: Action(req, controller), _ino(ino), _size(size), _off(off)
{
	Q_UNUSED(fi);
	startAsync();
}

ReadAction::~ReadAction()
{
	_ino = 0;
	_size = 0;
	_off = 0;
}

void ReadAction::asyncAction()
{
	File *file = contentContainer()->getFileByID(_ino);
	if(file)
		controller()->http()->get(file->getUrl(), _off, _size, new CallbackWithArgument<ReadAction, QByteArray>(this, &ReadAction::success, &ReadAction::error));
	else
		finishWithError(ENOENT);
}

void ReadAction::success(QByteArray data)
{
	finishWithBuffer(data);
}

void ReadAction::error(QString error)
{
	qWarning() << "Failed to retrieve contents of a file: " << error;
	finishWithError(EINVAL);
}
