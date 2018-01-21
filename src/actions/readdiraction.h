#ifndef READDIRACTION_H
#define READDIRACTION_H

#include "asyncaction.h"

class ReadDirAction : public AsyncAction
{
public:
	ReadDirAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi, Controller *controller);

	// AsyncAction interface
protected:
	void onLoaded(ContentItem *item) override;

private:
	size_t _size;
	off_t _off;

};

#endif // READDIRACTION_H
