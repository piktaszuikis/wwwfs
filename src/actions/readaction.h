#ifndef READACTION_H
#define READACTION_H

#include "asyncaction.h"
#include "../http/icontentcallback.h"

class ReadAction : public AsyncAction, public IContentCallback
{
public:
	ReadAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi, Controller *controller);

public:
	void setContent(QByteArray content) override;
	void setError(QString error) override;

private:
	size_t _size;
	off_t _off;

};

#endif // READACTION_H
