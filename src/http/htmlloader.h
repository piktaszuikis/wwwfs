#ifndef HTMLLOADER_H
#define HTMLLOADER_H

#include <QObject>
#include "icallback.h"
#include "../fs/folder.h"
#include "../controller.h"

class HtmlLoader : public QObject
{
	Q_OBJECT
public:
	explicit HtmlLoader(ICallback *callback, Folder* folder, Controller *controller, QObject *parent);
	virtual ~HtmlLoader();

private:
	void parseHtml(QByteArray content);
	void setResourceInfo(QSharedPointer<RemoteResourceInfo> resource, HttpItem *item);
	void resourceInfoFailed(QString error, HttpItem *item);

	void tryFinish();

private:
	ICallback *_callback;
	Controller *_controller;
	Folder* _folder;

	QList<HttpItem *> _itemsToLoad;
	int _itemsToLoadCount;
};

#endif // HTMLLOADER_H
