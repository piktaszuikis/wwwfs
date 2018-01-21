#include "httpitem.h"

HttpItem::HttpItem()
{

}

HttpItem::HttpItem(QString htmlTag, QString title, QString alt, QUrl url, size_t size)
	: _htmlTag(htmlTag), _title(title), _alt(alt), _url(url), _size(size)
{

}
