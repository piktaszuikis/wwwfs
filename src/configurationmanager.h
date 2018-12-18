#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "fs/rendererbase.h"
#include "fs/contentcontainer.h"

namespace ConfigurationManager
{
	extern const char *defaultRamSize;
	extern const long defaultRamLength;
	extern const char *defaultDiskSize;
	extern const char *defaultDiskDirectory;
	extern const int defaultRequestTimeoutMs;

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
