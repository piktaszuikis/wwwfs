#ifndef READDIRACTION_H
#define READDIRACTION_H

#include "action.h"

class ReadDirAction : public Action
{
	Q_OBJECT

public:
	ReadDirAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi, Controller *controller);
	virtual ~ReadDirAction();

protected:
	virtual void asyncAction() override;

private:
	void success();
	void error(QString error);

private:
	off_t _off;
	size_t _size;
	Folder *_folder;
};

#endif // READDIRACTION_H
