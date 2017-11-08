#define FUSE_USE_VERSION 31

#include <fuse.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <fcntl.h>
#include <stddef.h>
#include <assert.h>

#include "wwwfs-curl.h"
#include "wwwfs-parser.h"

/*
 * Command line options
 *
 * We can't set default values for the char* fields here because
 * fuse_opt_parse would attempt to free() them when the user specifies
 * different values on the command line.
 */
static struct options {
	const char *url;
	int show_help;
} options;

#define OPTION(t, p)                           \
{ t, offsetof(struct options, p), 1 }
static const struct fuse_opt option_spec[] = {
	OPTION("--url=%s", url),
	OPTION("-h", show_help),
	OPTION("--help", show_help),
	FUSE_OPT_END
};

static DirItem *private_data = 0;

static void *wwwfs_init(struct fuse_conn_info *conn, struct fuse_config *cfg)
{
	fprintf(stderr, "wwwfs_init");

	cfg->kernel_cache = 1;
	return 0;
}

static int wwwfs_getattr(const char *path, struct stat *stbuf, struct fuse_file_info *fi)
{
	int res = 0;

	fprintf(stderr, path);

	memset(stbuf, 0, sizeof(struct stat));

	if (strcmp(path, "/") == 0) {
			stbuf->st_mode = S_IFDIR | 0755;
			stbuf->st_nlink = 2;
	}else{

		DirItem *parsed = private_data;
		DirItem * found = 0;

		while(parsed){
			if(strcmp(path+1, parsed->name) == 0){
				found = parsed;
				break;
			}

			parsed = parsed->next;
		}

		if(found){

			if(found->isFolder)
			{
				stbuf->st_mode = S_IFDIR | 0755;
				stbuf->st_nlink = 2;
			}
			else
			{
				stbuf->st_mode = S_IFREG | 0444;
				stbuf->st_nlink = 1;
				stbuf->st_size = found->fileSize;
			}

		}else{
			res = -ENOENT;
		}
	}


	/*
		if (strcmp(path, "/") == 0) {
				stbuf->st_mode = S_IFDIR | 0755;
				stbuf->st_nlink = 2;

		} else if (strcmp(path+1, options.filename) == 0) {
				stbuf->st_mode = S_IFREG | 0444;
				stbuf->st_nlink = 1;
				stbuf->st_size = strlen(options.contents);
		} else
				res = -ENOENT;
		*/

	return res;
}

static int wwwfs_readdir(const char *path, void *buf, fuse_fill_dir_t filler,
						 off_t offset, struct fuse_file_info *fi,
						 enum fuse_readdir_flags flags)
{
	//(void) offset;
	//(void) fi;
	//(void) flags;

	fprintf(stderr, "wwwfs_readdir");

	if (strcmp(path, "/") != 0)
		return -ENOENT;

	filler(buf, ".", NULL, 0, 0);
	filler(buf, "..", NULL, 0, 0);

	DirItem *parsed = private_data;
	while(parsed){
		filler(buf, parsed->name, NULL, 0, 0);
		parsed = parsed->next;
	}

	fprintf(stderr, "/wwwfs_readdir");

	//filler(buf, options.filename, NULL, 0, 0);
	return 0;
}

static int wwwfs_open(const char *path, struct fuse_file_info *fi)
{
	if ((fi->flags & O_ACCMODE) != O_RDONLY)
	        return -EACCES;

	DirItem *parsed = private_data;
	int contains = 0;

	while(parsed){
		if(strcmp(path+1, parsed->name) == 0){
			contains = 1;
			break;
		}

		parsed = parsed->next;
	}

	if(!contains)
		return -ENOENT;

	//if (strcmp(path+1, options.filename) != 0)
	//        return -ENOENT;

	return 0;
}

static int wwwfs_read(const char *path, char *buf, size_t size, off_t offset,
					  struct fuse_file_info *fi)
{
	DirItem *parsed = private_data;
	DirItem *found = 0;

	while(parsed)
	{
		if(strcmp(path+1, parsed->name) == 0)
		{
			found = parsed;
			break;
		}

		parsed = parsed->next;
	}

	if(found)
	{
		WebData *data = getWebData(parsed->url);
		if(data && offset < data->size)
		{
			if(offset + size > data->size)
				size = data->size - offset;

			memcpy(buf, (char*)data->data + offset, size);
			data->dispose(data);

			return size;
		}

		if(data)
			data->dispose(data);
	}

	/*
	len = strlen(options.contents);
	if (offset < len) {
		if (offset + size > len)
			size = len - offset;
		memcpy(buf, options.contents + offset, size);
	} else
		size = 0;
	*/

	return 0;
}

static struct fuse_operations wwwfs_oper = {
	.init           = wwwfs_init,
	.getattr        = wwwfs_getattr,
	.readdir        = wwwfs_readdir,
	.open           = wwwfs_open,
	.read           = wwwfs_read,
};

static void show_help(const char *progname)
{
	printf("usage: %s [options] <mountpoint>\n\n", progname);
	printf("File-system specific options:\n"
		   "    --url=<s>          URL to mount"
		   "\n");
}

int wwwfs_fuse_init(int argc, char *argv[])
{
	struct fuse_args args = FUSE_ARGS_INIT(argc, argv);
	options.url = 0;

	if (fuse_opt_parse(&args, &options, option_spec, NULL) == -1)
		return 1;

	if (options.show_help || !options.url) {
		show_help(argv[0]);
		assert(fuse_opt_add_arg(&args, "--help") == 0);
		args.argv[0] = (char*) "";
	}


	WebData *data = getWebData((char *)options.url);
	DirItem *parsed = parseHTML(data);
	//data->dispose(data);

	private_data = parsed;

	while(parsed){
		printf("%c | %d\t| %-10s\t|\t%s\n", parsed->isFolder ? 'D' : 'F', parsed->fileSize, parsed->name, parsed->url);
		parsed = parsed->next;
	}

	//return 0;
	return fuse_main(args.argc, args.argv, &wwwfs_oper, NULL);
}
