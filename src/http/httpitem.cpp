#include "httpitem.h"

HttpItem::HttpItem(QString htmlTag, QString title, QString alt, QUrl url)
	: _htmlTag(htmlTag), _title(title), _alt(alt), _size(0)
{
	_url = url.adjusted(QUrl::RemoveFragment);
}
