#ifndef HTTPITEM_H
#define HTTPITEM_H

#include <QString>
#include <QUrl>

class HttpItem
{
public:
	HttpItem(QString htmlTag, QString title, QString alt, QUrl url);

	inline QString getHtmlTag() const { return _htmlTag; }
	inline QString getTitle() const { return _title; }
	inline QString getAlt() const { return _alt; }
	inline QString getMime() const {return _mime; }
	inline QUrl getUrl() const { return _url; }
	inline size_t getSize() const {return _size; }

	inline void setSize(const size_t size) { _size = size; }
	inline void setMime(const QString mime) { _mime = mime; }

private:
	QString _htmlTag;
	QString _title;
	QString _alt;
	QString _mime;
	QUrl _url;
	size_t _size;
};

#endif // HTTPITEM_H
