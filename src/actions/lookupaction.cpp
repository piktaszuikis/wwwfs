#include "lookupaction.h"
#define MAX_TIMEOUT 10000

LookupAction::LookupAction(fuse_req_t req, fuse_ino_t parent, const char *name, Controller *controller)
	: SyncAction(req, controller)
{
	Folder *folder = contentContainer()->getFolderByID(parent);

	if(folder)
	{
		ContentItem *item = folder->getByName(QString::fromUtf8(name));

		if(item)
		{
			struct fuse_entry_param entry =
			{
				.ino = item->nodeID(),
				.generation = 0,
				.attr = {},
				.attr_timeout = MAX_TIMEOUT,  //size timeout
				.entry_timeout = MAX_TIMEOUT, //filename timeout
			};

			item->wfuse_stat(&entry.attr);
			finishWithEntry(&entry);
			return;
		}
	}

	finishWithError(ENOENT);
}
