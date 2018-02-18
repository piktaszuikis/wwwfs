#include "readaction.h"
#include <QDebug>

ReadAction::ReadAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi, Controller *controller)
	: AsyncAction(req, controller), _size(size), _off(off)
{
	Q_UNUSED(fi);

	File *file = contentContainer()->getFileByID(ino);
	if(file)
		controller->http()->getContent2(file->getUrl(), off, size, this);
	else
		finishWithError(ENOENT);
}

void ReadAction::setContent(QByteArray content)
{
	finishWithBuffer(content);
}

void ReadAction::setError(QString error)
{
	qWarning() << "Failed to retrieve contents of a file: " << error;
	finishWithError(ECOMM);
}
