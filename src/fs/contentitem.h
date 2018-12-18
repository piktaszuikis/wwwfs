#ifndef CONTENTITEM_H
#define CONTENTITEM_H

#include <fuse_lowlevel.h>
#include <QString>
#include <QUrl>

class Folder;

/*absract*/ class ContentItem
{
public:
	ContentItem(fuse_ino_t node_id, QString name, QUrl url);
	virtual ~ContentItem();

	QString name() const;
	void setName(const QString name);

	Folder *parent() const;
	void setParent(Folder *parent);

	QUrl getUrl() const;
	void setUrl(const QUrl url);

	fuse_ino_t nodeID() const;

	void wfuse_stat(struct stat *st);
	struct stat *wfuse_stat();

	QString filename();
	QString path();

protected:
	void clearCache();
	virtual void wfuse_apply_stat(struct stat *st) = 0;

protected:
	fuse_ino_t _nodeID;

private:
	struct stat *_cachedStat;
	QString _name;
	QUrl _url;
	Folder *_parent;
};

#endif // CONTENTITEM_H
