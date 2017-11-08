#ifndef WWW_CURL

typedef struct WebData{
	int isHTML;
	char *url;
	char *encoding;

	void* data;
	long long size;

	void (*dispose)(struct WebData *);
} WebData;

WebData *getWebData(char *url);
int isHtml(char *url, long *fileSize);

#define WWW_CURL
#endif
