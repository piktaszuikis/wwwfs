#include "readdiraction.h"

ReadDirAction::ReadDirAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi, Controller *controller)
	: AsyncAction(req, controller), _off(off), _size(size)
{
	if(off < 0)
	{
		finishWithBuffer(nullptr, 0);
		return;
	}

	Folder *folder = contentContainer()->getFolderByID(ino);
	if(folder)
	{
		if(folder->isLoaded())
		{
			onLoaded(folder);
		}
		else
		{
			throw "NotImplementedException";
		}
	}
	else
	{
		finishWithError(ENOTDIR);
	}
}


void ReadDirAction::onLoaded(ContentItem *item)
{
	Folder *folder =  dynamic_cast<Folder *>(item);

	auto entries = folder->wfuse_get_direntries();

	if(!entries.isEmpty())
	{
		if(_off == 0 && _size > entries.size())
		{
			finishWithBuffer(entries.constData(), entries.size());
		}
		else
		{
			auto data = entries.mid(_off, _size);
			finishWithBuffer(data);
		}
	}
	else
	{
		finishWithBuffer(nullptr, 0);
	}
}
