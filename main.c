#include <stdio.h>
#include "wwwfs-fuse.h"

int main(int argc, char *argv[])
{
	return wwwfs_fuse_init(argc, argv);
}
