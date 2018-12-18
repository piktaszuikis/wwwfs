#include "readlinkaction.h"

ReadLinkAction::ReadLinkAction(fuse_req_t req, fuse_ino_t ino, Controller *controller)
	: Action(req, controller)
{
	Symlink *link = contentContainer()->getSymlinkByID(ino);

	if(link)
	{
		QString path = link->link();

		if(!path.isEmpty())
			finishWithLink(path);
		else
			finishWithError(ENOLINK);

		return;
	}

	finishWithError(ENOENT);
}
