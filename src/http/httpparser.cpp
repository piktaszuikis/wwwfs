#include "httpparser.h"
#include "httpitemcollectionloader.h"

#include <QNetworkReply>

//https://github.com/lagner/QGumboParser

QList<HttpItem *> HttpParser::parse(QByteArray html)
{
	Q_UNUSED(html);

	QList<HttpItem *>result;

	result.append(new HttpItem("a", "archive", "", QUrl("http://127.0.0.1/archive"), 0));
	result.append(new HttpItem("a", "arnie", "", QUrl("http://127.0.0.1/arnie"), 0));
	result.append(new HttpItem("a", "films", "", QUrl("http://127.0.0.1/films"), 0));
	result.append(new HttpItem("img", "0.jpg", "", QUrl("http://127.0.0.1/qt/0.jpg"), 0));

	return result;
}
