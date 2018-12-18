#include "controller.h"

#include "actions/getattraction.h"
#include "actions/lookupaction.h"
#include "actions/openaction.h"
#include "actions/readaction.h"
#include "actions/readdiraction.h"
#include "actions/readlinkaction.h"

Controller::Controller()
	: _content(nullptr), _http(nullptr)
{

}

void Controller::getAttr(fuse_req_t req, fuse_ino_t ino, fuse_file_info *fi)
{
	new GetAttrAction(req, ino, fi, this);
}

void Controller::lookup(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	new LookupAction(req, parent, name, this);
}

void Controller::open(fuse_req_t req, fuse_ino_t ino, fuse_file_info *fi)
{
	new OpenAction(req, ino, fi, this);
}

void Controller::read(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi)
{
	new ReadAction(req, ino, size, off, fi, this);
}

void Controller::readDir(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi)
{
	new ReadDirAction(req, ino, size, off, fi, this);
}

void Controller::readLink(fuse_req_t req, fuse_ino_t ino)
{
	new ReadLinkAction(req, ino, this);
}

Controller *Controller::get(fuse_req_t req)
{
	try
	{
		return get(fuse_req_userdata(req));
	}
	catch(...)
	{
		fuse_reply_err(req, EFAULT);
		throw;
	}
}

Controller *Controller::get(void *userdata)
{
	Controller *result = static_cast<Controller *>(userdata);

	if(!result)
		throw "Controller not found!";

	return result;
}

void Controller::run()
{
	_content = new ContentContainer();
	_http = new HttpClient();

	exec();
}
