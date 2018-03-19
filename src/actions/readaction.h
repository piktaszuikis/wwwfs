#ifndef READACTION_H
#define READACTION_H

#include "syncaction.h"
#include "../http/icontentcallback.h"

class ReadAction : public SyncAction
{
public:
	ReadAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi, Controller *controller);
	virtual ~ReadAction();

protected:
	virtual void asyncAction() override;

private:
	void success(QByteArray data);
	void error(QString error);


private:
	fuse_ino_t _ino;
	size_t _size;
	off_t _off;

};

#endif // READACTION_H
