#include "controller.h"

#include "actions/getattraction.h"
#include "actions/lookupaction.h"
#include "actions/openaction.h"
#include "actions/readaction.h"
#include "actions/readdiraction.h"

Controller::Controller()
	: _content(0), _http(0)
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





	/*

	//TODO: remove
	Folder *root = _content->getFolderByID(FUSE_ROOT_ID);

	{
		auto add = _content->beginAdd(root);

		QByteArray data = "Textas 1 asdf\n";
		File *file = dynamic_cast<File*>(add->addItem(new HttpItem("txt", "tekstas.txt", "", QUrl("https://127.0.0.1/aa.txt"), data.size())));
		file->setContent(data);
		file->setIsLoaded(true);


		data = "Textas 2 asdf\n";
		file = dynamic_cast<File*>(add->addItem(new HttpItem("txt", "", "", QUrl("https://127.0.0.1/aa1.txt"), data.size())));
		file->setContent(data);
		file->setIsLoaded(true);

	}
*/

	exec();
}
