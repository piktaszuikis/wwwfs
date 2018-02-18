#include "readdiraction.h"
#include "../http/httpitemcollectionloader.h"

ReadDirAction::ReadDirAction(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, fuse_file_info *fi, Controller *controller)
	: AsyncAction(req, controller), _off(off), _size(size)
{
	Q_UNUSED(fi);

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
			loaded(folder);
		}
		else
		{
			HttpItemCollectionLoader *loader = new HttpItemCollectionLoader(folder, controller, this);
			connect(loader, &HttpItemCollectionLoader::loaded, this, &ReadDirAction::loaded);
			connect(loader, &HttpItemCollectionLoader::error, this, &ReadDirAction::loadFailed);

			emit controller->http()->getContent(folder->getUrl(), loader);
		}
	}
	else
	{
		finishWithError(ENOTDIR);
	}
}

void ReadDirAction::loaded(Folder *folder)
{
	auto entries = folder->wfuse_get_direntries();

	if(folder)
	{
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
	else
	{
		finishWithError(EINVAL);
	}
}

void ReadDirAction::loadFailed(Folder *folder)
{
	if(folder)
		folder->setIsLoaded(true);

	finishWithError(EINVAL);
}

/*

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
*/
