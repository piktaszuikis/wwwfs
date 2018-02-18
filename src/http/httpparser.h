#ifndef HTTPPARSER_H
#define HTTPPARSER_H

#include <QObject>
#include <QList>
#include <QNetworkReply>

#include "httpitem.h"

namespace HttpParser
{
	QList<HttpItem *> parse(const QUrl &url, QByteArray html);
}

#endif // HTTPPARSER_H
