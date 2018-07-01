#include <QStandardPaths>
#include <QRegularExpression>
#include <QDebug>

#include <fuse_lowlevel.h>
#include <fuse_opt.h>

#include "configurationmanager.h"
#include "fs/humanreadablerenderer.h"

namespace ConfigurationManager
{
	QUrl _url;
	long _ramSize;
	long _ramLength;
	long _diskSize;
	QString _diskCacheDirectory;
	bool _avoidThumbnails;

	struct args_config {
		char *ramSize;
		long ramLength;
		char *diskSize;
		char *diskPath;
		int avoidThumbnails;
	};

	#define FS_OPT(t, p, v) { t, offsetof(struct args_config, p), v }

	static struct fuse_opt option_parser_list[] =
	{
		FS_OPT("cache-ram-size=%s", ramSize, 0),
		FS_OPT("cache-ram-length=%i", ramLength, 0),
		FS_OPT("cache-disk-size=%s", diskSize, 0),
		FS_OPT("cache-disk-path=%s", diskPath, 0),
		FS_OPT("avoid-thumbnails", avoidThumbnails, 1),
		FUSE_OPT_END
	};

	long parseBytes(const char *data, const char* defaultValue)
	{
		if(!data && defaultValue)
			return parseBytes(defaultValue, nullptr);

		if(!data)
			return 0;

		QRegularExpression re("(\\d+)([GMKB]?)B?", QRegularExpression::CaseInsensitiveOption);
		auto match = re.match(data);

		if(!match.hasMatch()) {
			fprintf(stderr, "Failed to parse size '%s'.", data);
			exit(1);
		}

		long result = match.captured(1).toLong();
		QString multiplyer = match.captured(2).toUpper();

		if(multiplyer == "K")
			result *= 1024;
		else if(multiplyer == "M")
			result *= 1024 * 1024;
		else if(multiplyer == "G")
			result *= 1024 * 1024 * 1024;

		return result;
	}

	static int parse_option(void *, const char *arg, int key, struct fuse_args *)
	{
		if(key == FUSE_OPT_KEY_NONOPT && _url.isEmpty())
		{
			_url = QString::fromUtf8(arg);
			return 0;
		}

		return 1;
	}

	void init(fuse_args *args)
	{
		struct args_config conf = {};
		fuse_opt_parse(args, &conf, option_parser_list, parse_option);

		if(!_url.isEmpty())
		{
			if(_url.scheme().isEmpty()){
				qDebug() << "[URL]" << _url.toString(QUrl::None);
				_url = "https://" + _url.toString(QUrl::None);
				qDebug() << "[URL]" << _url.toString(QUrl::None);
				//_url.setScheme("https");
			}
		}

		_ramLength = conf.ramLength > 0 ? conf.ramLength : defaultRamLength;
		_ramSize = parseBytes(conf.ramSize, defaultRamSize);
		_diskSize = parseBytes(conf.diskSize, defaultDiskSize);
		_diskCacheDirectory = QString(conf.diskPath);
		_avoidThumbnails = conf.avoidThumbnails != 0;

		if(_diskCacheDirectory.isEmpty())
			_diskCacheDirectory = QStandardPaths::writableLocation(QStandardPaths::CacheLocation);
	}

	RendererBase *createRenderer(ContentContainer *container, Folder *parent)
	{
		return new HumanReadableRenderer(container, parent);
	}

	QUrl rootUrl()
	{
		return _url;
	}

	int sampleSize()
	{
		return 1024 * 1024; //1M
	}

	long cacheRamSize()
	{
		return _ramSize;
	}

	long cacheRamLength()
	{
		return _ramLength;
	}

	QString cacheDiskDirectory()
	{
		return _diskCacheDirectory;
	}

	qint64 cacheDiskSize()
	{
		return _diskSize;
	}

	bool isAvoidThumbnails()
	{
		return _avoidThumbnails;
	}

}


