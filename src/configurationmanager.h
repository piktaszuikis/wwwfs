#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "fs/rendererbase.h"
#include "fs/contentcontainer.h"

namespace ConfigurationManager
{
	void init();
	RendererBase *createRenderer(ContentContainer *container, Folder *parent);
	QUrl rootUrl();

	int sampleSize();
	long cacheRamSize();
	long cacheRamLength();

	QString cacheDiskDirectory();
	qint64 cacheDiskSize();

}



#endif // CONFIGURATIONMANAGER_H
