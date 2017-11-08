#include "wwwfs-curl.h"

typedef struct DirItem {
	int isFolder;
	long fileSize;
	char *name;
	char *url;

	struct DirItem *next;
	struct DirItem *prev;
} DirItem;

DirItem *parseHTML(WebData *data);
