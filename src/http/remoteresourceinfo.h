#ifndef REMOTERESOURCEINFO_H
#define REMOTERESOURCEINFO_H

#include <QString>

class RemoteResourceInfo
{
public:
	explicit RemoteResourceInfo(QString mime, size_t size) : mime(mime), size(size) { }

	QString mime;
	size_t size;
};

#endif // REMOTERESOURCEINFO_H
