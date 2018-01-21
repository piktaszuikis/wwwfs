#include "readaction.h"

ReadAction::ReadAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi, Controller *controller)
	: AsyncAction(req, controller), _size(size), _off(off)
{
	Q_UNUSED(fi);

	File *file = contentContainer()->getFileByID(ino);
	if(file)
	{
		if(file->isLoaded())
			onLoaded(file);
		else
		{
			throw "NotImplementedException";
			//file->load(this);
		}
	}
	else
	{
		fuse_reply_err(req, ENOENT);
	}
}

void ReadAction::onLoaded(ContentItem *item)
{
	File *file = dynamic_cast<File *>(item);

	if(file)
		finishWithBuffer(file->content(_off, _size));
	else
		finishWithError(ECOMM);
}
