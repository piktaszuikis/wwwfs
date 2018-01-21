#ifndef OPENACTION_H
#define OPENACTION_H

#include "syncaction.h"

class OpenAction : SyncAction
{
public:
	OpenAction(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi, Controller *controller);
};

#endif // OPENACTION_H
