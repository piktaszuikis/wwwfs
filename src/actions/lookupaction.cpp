#include "lookupaction.h"

#include "../http/htmlloader.h"
#include "callback.h"

#define MAX_TIMEOUT 10000

LookupAction::LookupAction(fuse_req_t req, fuse_ino_t parentID, const char *name, Controller *controller)
	: SyncAction(req, controller), _parent(0), _name(0)
{
	_name = name;
	_parent = contentContainer()->getFolderByID(parentID);

	if(_parent)
	{
		if(_parent->isLoaded())
			success();
		else
			startAsync();

		return;
	}

	finishWithError(ENOENT);
}

LookupAction::~LookupAction()
{
	_parent = nullptr;
	_name = nullptr;
}

void LookupAction::asyncAction()
{
	new HtmlLoader(new Callback<LookupAction>(this, &LookupAction::success, &LookupAction::error), _parent, controller(), this);
}

void LookupAction::success()
{
	ContentItem *item = _parent->getByName(QString::fromUtf8(_name));

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

	qDebug() << QString("file not found: '%1' in folder '%2'.").arg(_name).arg(_parent->name());
	finishWithError(ENOENT);
}

void LookupAction::error(QString error)
{
	qWarning() << error;
	finishWithError(EINVAL);
}

