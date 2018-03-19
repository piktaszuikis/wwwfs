#ifndef LOOKUPACTION_H
#define LOOKUPACTION_H

#include "syncaction.h"

class LookupAction : public SyncAction
{
public:
	LookupAction(fuse_req_t req, fuse_ino_t parent, const char *name, Controller *controller);
	virtual ~LookupAction();

protected:
	virtual void asyncAction() override;

private:
	void success();
	void error(QString error);

	Folder *_parent;
	const char *_name;

};

#endif // LOOKUPACTION_H
