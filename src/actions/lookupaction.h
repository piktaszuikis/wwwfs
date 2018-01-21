#ifndef LOOKUPACTION_H
#define LOOKUPACTION_H

#include "syncaction.h"

class LookupAction : public SyncAction
{
public:
	LookupAction(fuse_req_t req, fuse_ino_t parent, const char *name, Controller *controller);
};

#endif // LOOKUPACTION_H
