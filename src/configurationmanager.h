#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "fs/rendererbase.h"
#include "fs/contentcontainer.h"

namespace ConfigurationManager
{
	static const char *defaultRamSize = "200MB";
	static const long defaultRamLength = 10000L;
	static const char *defaultDiskSize = "400MB";
	static const char *defaultDiskDirectory = "~/.cache/wwwfs";
	static const int defaultRequestTimeoutMs = 5000;

	void init(struct fuse_args *args);
	QUrl rootUrl();
	RendererBase *createRenderer(ContentContainer *container, Folder *parent);

	int sampleSize();
	long cacheRamSize();
	long cacheRamLength();

	QString cacheDiskDirectory();
	qint64 cacheDiskSize();
	bool isAvoidThumbnails();
	int requestTimeoutMs();

}



#endif // CONFIGURATIONMANAGER_H
