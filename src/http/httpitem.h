#ifndef HTTPITEM_H
#define HTTPITEM_H

#include <QString>
#include <QUrl>

class HttpItem
{
public:
	HttpItem();
	HttpItem(QString htmlTag, QString title, QString alt, QUrl url, size_t size);

	inline QString getHtmlTag() const { return _htmlTag; }
	inline QString getTitle() const { return _title; }
	inline QString getAlt() const { return _alt; }
	inline QUrl getUrl() const { return _url; }
	inline size_t getSize() const {return _size; }

private:
	QString _htmlTag;
	QString _title;
	QString _alt;
	QUrl _url;
	size_t _size;
};

#endif // HTTPITEM_H
