#ifndef READDIRACTION_H
#define READDIRACTION_H

#include "asyncaction.h"

class ReadDirAction : public AsyncAction
{
	Q_OBJECT

public:
	ReadDirAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi, Controller *controller);

	// AsyncAction interface

public slots:
	void loaded(Folder *folder);
	void loadFailed(Folder *folder);

protected:
	//void onLoaded(ContentItem *item) override;

private:
	off_t _off;
	size_t _size;
};

#endif // READDIRACTION_H
