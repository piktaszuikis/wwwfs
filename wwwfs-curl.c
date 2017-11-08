#include <stdlib.h>
#include <string.h>
#include <curl/curl.h>
#include "wwwfs-curl.h"

typedef struct buffer
{
	void *data;
	size_t size;

	struct buffer* next;

	void (*add)(struct buffer*, void *data, size_t size);
	WebData *(*take)(struct buffer*, int isHtml, char *url);
} Buffer;

int endsWith(char *str, char *suffix)
{
	if(!str || !suffix)
		return 0;

	str = str + strlen(str) - strlen(suffix);

	return strncmp(str, suffix, strlen(suffix)) == 0;
}

void webDataDispose(WebData *self){
	if(self){
		free(self->data);
		free(self->url);

		self->data = 0;
		self->url = 0;
		self->size = -1;
	}
}

void bufferAdd(Buffer* self, void *data, size_t size){
	while(self && self->next){
		self = self->next;
	}

	self->next = calloc(1, sizeof(*self->next));

	self->next->data = malloc(size);
	self->next->size = size;
	memcpy(self->next->data, data, size);
}

WebData *bufferTake(Buffer *self, int isHtml, char *url){
	WebData *result = calloc(1, sizeof(*result));
	result->isHTML = isHtml;
	result->url = url;
	result->dispose = webDataDispose;

	Buffer* tmp = self;
	while(tmp)
	{
		result->size += tmp->size;
		tmp = tmp->next;
	}

	//allocate
	result->data = malloc(result->size);
	if(!result->data)
	{
		fprintf(stderr, "No enough memory: could not allocate %ld bytes. Pretending as if request was empty.");
		result->size = 0;
	}

	//copy data
	tmp = self;
	char* data = result->data;
	while(tmp)
	{
		memcpy(data, tmp->data, tmp->size);
		data += tmp->size;
		tmp = tmp->next;
	}

	// dispose
	while(self)
	{
		tmp = self->next;
		free(self->data);
		free(self);
		self = tmp;
	}

	return result;
}

Buffer *newBuffer(){
	Buffer *result = calloc(1, sizeof(*result));

	result->add = bufferAdd;
	result->take = bufferTake;

	return result;
}


static size_t write_callback(void *data, size_t size, size_t nmemb, void *callbackData)
{
	Buffer *buffer = (Buffer *)callbackData;
	buffer->add(buffer, data, size * nmemb);
	return size * nmemb;
}


WebData *getWebData(char *url){
	int isHtml = 0;
	Buffer *buffer = newBuffer();
	CURL *curl = curl_easy_init();

	if(!curl)
		return 0;

	struct curl_slist *list = NULL;
	//
	list = curl_slist_append(list, "Accept: text/html, application/xml;q=0.9, application/xhtml+xml, image/png, image/webp, image/jpeg, image/gif,*/*;q=0.1" );
	list = curl_slist_append(list, "User-Agent: wwwFS/0.0 (Linux)");
	curl_easy_setopt(curl, CURLOPT_HTTPHEADER, list);

	curl_easy_setopt(curl, CURLOPT_URL, url);
	curl_easy_setopt(curl, CURLOPT_WRITEFUNCTION, write_callback);
	curl_easy_setopt(curl, CURLOPT_WRITEDATA, buffer);
	curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
	curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
	//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

	int res = curl_easy_perform(curl);
	if(!res){
		char *data;
		res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &data);

		if(!res && data)
			isHtml = strstr(data, "html") != 0;

		res = curl_easy_getinfo(curl, CURLINFO_EFFECTIVE_URL, &data);

		if(!res && data)
			url = data;
	}

	url = strdup(url);
	curl_easy_cleanup(curl);

	return buffer->take(buffer, isHtml, url);
}

int isHtml(char *url, long *fileSize)
{
	if(!url)
		return 0;

	{ //performance optimizations
		if(endsWith(url, "htm") || endsWith(url, "html"))
			return 1;

		//if(endsWith(url, "png") || endsWith(url, "jpg") || endsWith(url, "jpeg") || endsWith(url, "gif"))
		//	return 0;
	}

	CURL *curl = curl_easy_init();

	if(curl)
	{
		curl_easy_setopt(curl, CURLOPT_URL, url);
		curl_easy_setopt(curl, CURLOPT_FOLLOWLOCATION, 1L);
		curl_easy_setopt(curl, CURLOPT_NOBODY, 1L);
		curl_easy_setopt(curl, CURLOPT_MAXREDIRS, 10L);
		//curl_easy_setopt(curl, CURLOPT_VERBOSE, 1L);

		int res = curl_easy_perform(curl);
		if(!res)
		{
			curl_off_t content_length;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_LENGTH_DOWNLOAD_T, &content_length);

			if(!res && content_length > 0)
				*fileSize = content_length;

			char *mime;
			res = curl_easy_getinfo(curl, CURLINFO_CONTENT_TYPE, &mime);

			if(!res && mime)
				return strstr(mime, "html") != 0;
		}
	}

	return 0;
}
