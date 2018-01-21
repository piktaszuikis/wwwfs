#ifndef CONFIGURATIONMANAGER_H
#define CONFIGURATIONMANAGER_H

#include "fs/rendererbase.h"
#include "fs/contentcontainer.h"

namespace ConfigurationManager
{
	void init();
	RendererBase *createRenderer(ContentContainer *container, Folder *parent);
	QUrl rootUrl();
}



#endif // CONFIGURATIONMANAGER_H
