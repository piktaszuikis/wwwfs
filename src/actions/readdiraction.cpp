#include "readdiraction.h"
#include "../http/htmlloader.h"
#include "callback.h"

ReadDirAction::ReadDirAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi, Controller *controller)
	: Action(req, controller), _off(off), _size(size), _folder(nullptr)
{
	Q_UNUSED(fi);

	if(off < 0)
	{
		finishWithBuffer(nullptr, 0);
		return;
	}

	_folder = contentContainer()->getFolderByID(ino);
	if(_folder)
	{
		if(_folder->isLoaded())
			success();
		else
			startAsync();

		return;
	}

	finishWithError(ENOTDIR);
}

ReadDirAction::~ReadDirAction()
{
	_folder = nullptr;
	_off = 0;
	_size = 0;
}

void ReadDirAction::asyncAction()
{
	new HtmlLoader(new Callback<ReadDirAction>(this, &ReadDirAction::success, &ReadDirAction::error), _folder, controller(), this);
}

void ReadDirAction::success()
{
	if(_folder)
	{
		auto entries = _folder->wfuse_get_direntries();

		if(!entries.isEmpty())
		{
			if(_off == 0 && _size > static_cast<size_t>(entries.size()))
			{
				finishWithBuffer(entries.constData(), static_cast<size_t>(entries.size()));
			}
			else
			{
				auto data = entries.mid(static_cast<int>(_off), static_cast<int>(_size));
				finishWithBuffer(data);
			}
		}
		else
		{
			finishWithBuffer(nullptr, 0);
		}
	}
	else
	{
		finishWithError(EINVAL);
	}
}

void ReadDirAction::error(QString error)
{
	qWarning() << error;
	finishWithError(EINVAL);
}
