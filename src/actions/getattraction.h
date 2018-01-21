#ifndef GETATTRACTION_H
#define GETATTRACTION_H

#include "syncaction.h"

class GetAttrAction : SyncAction
{
public:
	GetAttrAction(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi, Controller *controller);

};

#endif // GETATTRACTION_H
