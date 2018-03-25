#include <QStandardPaths>

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
		//return QUrl("http://anime-fanservice.org/coppermine/albums/C_galleries/");
	}

	int sampleSize()
	{
		return 1024 * 500; //500KB
	}

	long cacheRamSize()
	{
		return 1024L * 1024 * 50; //50MB
	}

	long cacheRamLength()
	{
		return 1000;
	}

	QString cacheDiskDirectory()
	{
		return QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	}

	qint64 cacheDiskSize()
	{
		return 1024L * 1024 * 200; //200 MB
	}


}


