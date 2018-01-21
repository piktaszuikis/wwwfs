#include "configurationmanager.h"
#include "fs/humanreadablerenderer.h"

namespace ConfigurationManager
{
	void init()
	{
//TODO: load configuration from command line options
	}

	RendererBase *createRenderer(ContentContainer *container, Folder *parent)
	{
		return new HumanReadableRenderer(container, parent);
	}

	QUrl rootUrl()
	{
		return QUrl("http://127.0.0.1/");
	}


}


