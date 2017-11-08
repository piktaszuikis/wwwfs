#include <string.h>
#include <stdio.h>
#include <libxml2/libxml/HTMLparser.h>
#include <libxml2/libxml/HTMLtree.h>
#include <libxml2/libxml/xpath.h>

#include "wwwfs-parser.h"

char *getHost(char *url){
	if(url)
	{
		int isSchema = 0;
		url = strdup(url);

		for(char *c = url; *c; c++)
		{
			if(c[0] == ':')
			{
				isSchema = 1;
				continue;
			}

			if(c[0] == '/')
			{
				if(!isSchema)
				{
					c[0] = 0;
					break;
				}

				continue;
			}

			isSchema = 0;
		}
	}

	return url;
}

char *concat(char *a, char *b){
	char *result = malloc(strlen(a) + strlen(b) + 1);
	if(!result){
		return 0;
	}

	strcpy(result, a);
	strcat(result, b);

	return result;
}

DirItem * newDirItem(DirItem *root, char *name, char *url)
{
	if(!url){
		free(name);
		return root;
	}

	if(strncmp("http", url, 4)){
		free(url);
		free(name);
		return root;
	}

	if(!name || strlen(name) <= 0){
		free(name);
		free(url);
		return root;
	}

	for(int i = strlen(name) - 1; i >= 0; i--){
		if(name[i] == '\n' || name[i] == '\r')
			name[i] = ' ';

		if(name[i] == '/')
			name[i] = '_';
	}

	DirItem *current = root;
	DirItem *prev = current;
	while(current)
	{
		if(strcmp(current->url, url) == 0)
		{
			free(name);
			free(url);
			return root;
		}

		prev = current;
		current = current->next;
	}

	DirItem * result = calloc(1, sizeof(*result));
	result->prev = prev;
	result->name = name;
	result->url = url;
	result->isFolder = isHtml(url, &result->fileSize);

	if(root){
		prev->next = result;
		return root;
	}

	return result;
}

DirItem *addItems(DirItem *result, xmlXPathContextPtr ctx, char* host, char *pattern, char *urlAttrName, char *nameAttrName)
{
	xmlXPathObjectPtr nodes = xmlXPathEval(pattern, ctx);
	if(nodes && nodes->nodesetval && nodes->nodesetval->nodeNr > 0){

		for(int i = 0; i < nodes->nodesetval->nodeNr; i++)
		{
			xmlNode * node = nodes->nodesetval->nodeTab[i];

			char *href = xmlGetProp(node, urlAttrName);
			if(href){
				char *hash = strrchr(href, '#');
				if(hash)
					*hash = 0;

				if(href[0] == '/' || href[0] == '?')
				{
					char *fullHref = concat(host, href);
					free(href);
					href = fullHref;
				}

				if(!strlen(href)){
					free(href);
					continue;
				}
			}

			char *name = 0;
			if(nameAttrName)
				name = xmlGetProp(node, nameAttrName);

			if(!name)
				name = xmlNodeGetContent(node);

			if((!name || strlen(name) == 0) && href)
			{
				name = strrchr(href, '/');
				if(name)
				{
					name = strdup(name + 1);
				}
			}

			result = newDirItem(result, name, href);

			node = node->next;
		}

	}

	return result;
}

DirItem *parseHTML(WebData *data)
{
	DirItem *result = 0;

	htmlDocPtr doc = htmlReadMemory(data->data, data->size, "", data->encoding, HTML_PARSE_RECOVER | HTML_PARSE_NOERROR | HTML_PARSE_NOWARNING);

	xmlXPathContextPtr ctx = xmlXPathNewContext(doc);
	if(!ctx){
		fprintf(stderr, "Failed to parse html: could not create xpath context.");
		return 0;
	}

	char *host = getHost(data->url);

	result = addItems(result, ctx, host, "//a", "href", 0);
	result = addItems(result, ctx, host, "//img", "src", "alt");

	free(host);
	xmlXPathFreeContext(ctx);

	return result;
}
