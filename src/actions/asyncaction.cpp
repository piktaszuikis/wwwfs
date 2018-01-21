#include "asyncaction.h"

AsyncAction::AsyncAction(fuse_req_t req, Controller *thread)
	: SyncAction(req, thread)
{
	moveToThread(thread);
}

void AsyncAction::loaded(ContentItem *item)
{
	if(item)
		onLoaded(item);
	else
		finishWithError(ECOMM);
}
