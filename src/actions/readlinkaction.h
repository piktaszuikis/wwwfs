#ifndef READLINKACTION_H
#define READLINKACTION_H

#include "action.h"

class ReadLinkAction : Action
{
public:
	ReadLinkAction(fuse_req_t req, fuse_ino_t ino, Controller *controller);

};

#endif // READLINKACTION_H
