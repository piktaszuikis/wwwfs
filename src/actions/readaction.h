#ifndef READACTION_H
#define READACTION_H

#include "asyncaction.h"

class ReadAction : public AsyncAction
{
public:
	ReadAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi, Controller *controller);

protected:
	virtual void onLoaded(ContentItem *item) override;

private:
	size_t _size;
	off_t _off;

};

#endif // READACTION_H
