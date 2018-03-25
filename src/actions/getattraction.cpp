#include "getattraction.h"
#define MAX_TIMEOUT 10000

GetAttrAction::GetAttrAction(fuse_req_t req, fuse_ino_t ino, fuse_file_info *fi, Controller *controller)
	: Action(req, controller)
{
	Q_UNUSED(fi);

	ContentItem *item = contentContainer()->getByID(ino);

	if(item)
		finishWithAttr(item->wfuse_stat(), MAX_TIMEOUT);
	else
		finishWithError(ENOENT);

}
