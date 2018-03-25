#include "openaction.h"

OpenAction::OpenAction(fuse_req_t req, fuse_ino_t ino, fuse_file_info *fi, Controller *controller)
	: Action(req, controller)
{
	if((fi->flags & O_ACCMODE) != O_RDONLY)
	{
		finishWithError(EACCES);
	}
	else
	{
		ContentItem *item = contentContainer()->getByID(ino);

		if(item)
		{
			if(!dynamic_cast<Folder *>(item)) //is it a file?
				finishWithOpen(fi);
			else
				finishWithError(EISDIR);
		}
		else
		{
			finishWithError(ENOENT);
		}
	}
}
