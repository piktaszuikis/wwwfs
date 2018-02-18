#ifndef CONTROLLER_H
#define CONTROLLER_H

#include <QThread>
#include <fuse_lowlevel.h>
#include "http/httpclient.h"
#include "fs/contentcontainer.h"

class Controller : public QThread
{
public:
	Controller();
	void getAttr(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
	void lookup(fuse_req_t req, fuse_ino_t parent, const char *name);
	void open(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi);
	void read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);
	void readDir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi);


	inline ContentContainer *contentContainer() const { return _content; }
	inline HttpClient * http() const { return _http; }

	static Controller *get(fuse_req_t req);
	static Controller *get(void *userdata);

protected:
	void run();

private:
	ContentContainer *_content;
	HttpClient *_http;

};

#endif // CONTROLLER_H
