#include <fuse_lowlevel.h>

#include <QCoreApplication>
#include <QElapsedTimer>
#include <QDebug>

#include "fs/folder.h"
#include "fs/file.h"

#include "fs/contentcontainer.h"
#include "controller.h"

#include "callbackwithargument.h"


#include "http/cache/cacheditem.h"


#define MAX_TIMEOUT 10000

static void hello_ll_getattr2(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	Controller::get(req)->getAttr(req, ino, fi);
/*
	ContentItem *item = _root->getByID(ino);

	if(item)
	{
		struct stat stat_buf = {};
		item->wfuse_stat(&stat_buf);
		fuse_reply_attr(req, &stat_buf, MAX_TIMEOUT);
	}
	else
	{
		fuse_reply_err(req, ENOENT);
	}
	*/
}

static void hello_ll_lookup2(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	Controller::get(req)->lookup(req, parent, name);

	/*
	Folder *folder = _root->getFolderByID(parent);
	if(folder)
	{
		QString filename = QString::fromUtf8(name);

		foreach(auto item, folder->contents())
		{
			if(item->name() == filename)
			{
				struct fuse_entry_param entry = {
					.ino = item->nodeID(),
					.attr_timeout = MAX_TIMEOUT,  //size timeout
					.entry_timeout = MAX_TIMEOUT, //filename timeout
				};

				item->wfuse_stat(&entry.attr);
				fuse_reply_entry(req, &entry);
				return;
			}
		}

	}

	fuse_reply_err(req, ENOENT);
	*/
}

static void hello_ll_open2(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	Controller::get(req)->open(req, ino, fi);

/*
	if((fi->flags & 3) != O_RDONLY)
	{
		fuse_reply_err(req, EACCES);
	}
	else
	{
		auto item = _root->getByID(ino);

		if(item)
		{
			if(!dynamic_cast<Folder *>(item))
				fuse_reply_open(req, fi);
			else
				fuse_reply_err(req, EISDIR);
		}
		else
		{
			fuse_reply_err(req, ENOENT);
		}
	}
	*/
}

static void hello_ll_read2(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	Controller::get(req)->read(req, ino, size, off, fi);
	/*
	File *file = _root->getFileByID(ino);
	if(file)
	{
		auto data = file->content(off, size);

		if(data.isEmpty())
			fuse_reply_buf(req, NULL, 0);
		else
			fuse_reply_buf(req, data.data(), data.size());
	}
	else
	{
		fuse_reply_err(req, ENOENT);
	}
	*/
}

static void hello_ll_readdir2(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	Controller::get(req)->readDir(req, ino, size, off, fi);
	/*
	if(off < 0)
	{
		fuse_reply_buf(req, NULL, 0);
		return;
	}

	Folder *folder = _root->getFolderByID(ino);
	if(folder)
	{
		auto entries = folder->wfuse_get_direntries();

		if(!entries.isEmpty())
		{
			if(off == 0 && size > entries.size())
			{
				fuse_reply_buf(req, entries.constData(), entries.size());
			}
			else
			{
				auto data = entries.mid(off, size);
				fuse_reply_buf(req, data.constData(), data.size());
			}
		}
		else
		{
			fuse_reply_buf(req, NULL, 0);
		}
	}
	else
	{
		fuse_reply_err(req, ENOTDIR);
	}
*/
}

static void init(void *userdata, struct fuse_conn_info *)
{
	Controller::get(userdata)->start();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);

	//QThread::sleep(2);

	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);

	struct fuse_cmdline_opts opts;
	int ret = -1;
	if (fuse_parse_cmdline(&args, &opts) != 0)
		return 1;
	if (opts.show_help) {
		printf("usage: %s [options] <mountpoint>\n\n", argv[0]);
		fuse_cmdline_help();
		fuse_lowlevel_help();
		ret = 0;
		goto err_out1;
	} else if (opts.show_version) {
		printf("FUSE library version %s\n", fuse_pkgversion());
		fuse_lowlevel_version();
		ret = 0;
		goto err_out1;
	}

	{
		struct fuse_lowlevel_ops fs_opererations = { };
		fs_opererations.init    = init;
		fs_opererations.lookup  = hello_ll_lookup2;
		fs_opererations.getattr = hello_ll_getattr2;
		fs_opererations.readdir = hello_ll_readdir2;
		fs_opererations.open    = hello_ll_open2;
		fs_opererations.read    = hello_ll_read2;

		struct fuse_session *se = fuse_session_new(&args, &fs_opererations, sizeof(fs_opererations), new Controller());
		if (se == NULL)
			goto err_out1;
		if (fuse_set_signal_handlers(se) != 0)
			goto err_out2;
		if (fuse_session_mount(se, opts.mountpoint) != 0)
			goto err_out3;

		fuse_daemonize(opts.foreground);

		/* Block until ctrl+c or fusermount -u */
		if (opts.singlethread)
			ret = fuse_session_loop(se);
		else
			ret = fuse_session_loop_mt(se, opts.clone_fd);

		fuse_session_unmount(se);
	err_out3:
		fuse_remove_signal_handlers(se);
	err_out2:
		fuse_session_destroy(se);
	}

err_out1:
	free(opts.mountpoint);
	fuse_opt_free_args(&args);

	return ret ? 1 : 0;
}
