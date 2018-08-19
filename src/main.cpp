#include <fuse_lowlevel.h>

#include <QCoreApplication>
#include <QDebug>

#include "configurationmanager.h"
#include "controller.h"

static void hello_ll_getattr2(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	Controller::get(req)->getAttr(req, ino, fi);
}

static void hello_ll_lookup2(fuse_req_t req, fuse_ino_t parent, const char *name)
{
	Controller::get(req)->lookup(req, parent, name);
}

static void hello_ll_open2(fuse_req_t req, fuse_ino_t ino, struct fuse_file_info *fi)
{
	Controller::get(req)->open(req, ino, fi);
}

static void hello_ll_read2(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	Controller::get(req)->read(req, ino, size, off, fi);
}

static void hello_ll_readdir2(fuse_req_t req, fuse_ino_t ino, size_t size, off_t off, struct fuse_file_info *fi)
{
	Controller::get(req)->readDir(req, ino, size, off, fi);
}

static void init(void *userdata, struct fuse_conn_info *)
{
	Controller::get(userdata)->start();
}

static void show_help(const char *program)
{
	fprintf(stderr, "usage: %s <url> <mountpoint> [options]\n\n", program);

	fprintf(stderr,
			"general options:\n"
			"    url              the address of a page to mount (https://www.example.org)\n"
			"    -o opt,[opt...]  mount options\n"
			"\n"
			"wwwfs options:\n"
	);

//	fprintf(stderr, "    -o renderer=            type of renderer to use\n");

	fprintf(stderr,
			"    -o cache-ram-size=NUM   how much RAM to use for caching. Accepts a number with a unit (default: %s)\n"
			"    -o cache-ram-length=NUM how many items to keep in memory (default: %ld)\n"
			"    -o cache-disk-size=NUM  how much disk space to use for caching. Accepts a number with a\n"
			"                            unit. If 0 - disk cache will be disabled. (default: %s)\n"
			"    -o cache-disk-path=NUM  where to store disk cache (default: %s)\n"
			"    -o avoid-thumbnails     skip links, containing substring 'thumb'\n"
			"    -o timeout=NUM          default timeout for HTTP requests in milliseconds (default: %d ms)\n"
			"\nfuse options:\n"
			,
			ConfigurationManager::defaultRamSize,
			ConfigurationManager::defaultRamLength,
			ConfigurationManager::defaultDiskSize,
			ConfigurationManager::defaultDiskDirectory,
			ConfigurationManager::defaultRequestTimeoutMs
	);

	fuse_cmdline_help();
	fuse_lowlevel_help();
}

int main(int argc, char *argv[])
{
	QCoreApplication app(argc, argv);
	//QThread::sleep(2);

	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	ConfigurationManager::init(&args);

	struct fuse_cmdline_opts opts;
	int ret = -1;
	if (fuse_parse_cmdline(&args, &opts) != 0)
		return 1;
	if (opts.show_help) {
		show_help(argv[0]);
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
		if (se == nullptr)
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
