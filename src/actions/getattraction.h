#ifndef GETATTRACTION_H
#define GETATTRACTION_H

#include "action.h"

class GetAttrAction : Action
{
public:
	GetAttrAction(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi, Controller *controller);

};

#endif // GETATTRACTION_H
