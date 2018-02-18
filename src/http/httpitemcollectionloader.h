#ifndef HTTPITEMCOLLECTIONLOADER_H
#define HTTPITEMCOLLECTIONLOADER_H

#include <QList>

#include "httpitem.h"
#include "icontentcallback.h"
#include "../controller.h"

class HttpItemCollectionLoader : public QObject, public IContentCallback
{
	Q_OBJECT
	friend class FileInfoLoader;

public:
	explicit HttpItemCollectionLoader(Folder *folder, Controller *controller, QObject *parent = nullptr);
	virtual ~HttpItemCollectionLoader();

signals:
	void loaded(Folder *folder);
	void error(Folder *folder);

public slots:
	// IContentCallback interface
	void setContent(QByteArray content) override;
	void setError(QString error) override;

protected:
	void setItemInfo(HttpItem *item, size_t size, QString mime);
	void setItemError(HttpItem *item, QString error);


private:
	void finishLoading();

	int _itemsToLoadCount;
	Folder *_folder;
	Controller *_controller;
	QList<HttpItem *> _itemsToLoad;
};

#endif // HTTPITEMCOLLECTIONLOADER_H
