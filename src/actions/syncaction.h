#ifndef ACTION_H
#define ACTION_H

#include <QObject>
#include <fuse_lowlevel.h>
#include "../controller.h"

typedef struct stat Stat;

/*abstract*/ class SyncAction : public QObject
{
	Q_OBJECT

public:
	SyncAction(fuse_req_t req, Controller *controller);

protected slots:
	int finishWithError(int error);
	int finishWithAttr(const Stat *attr, double attr_timeout);
	int finishWithEntry(const struct fuse_entry_param *e);
	int finishWithOpen(const struct fuse_file_info *fi);
	int finishWithBuffer(const char *buf, size_t size);
	int finishWithBuffer(QByteArray data);

protected:
	inline Controller *controller() const { return _controller; }
	inline ContentContainer *contentContainer() const { return controller()->contentContainer(); }

private:
	fuse_req_t _req;
	Controller *_controller;
	bool _isFinished;

};

#endif // ACTION_H