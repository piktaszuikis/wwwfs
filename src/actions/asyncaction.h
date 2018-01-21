#ifndef ASYNCACTION_H
#define ASYNCACTION_H

#include "syncaction.h"

/*abstract*/ class AsyncAction : public SyncAction
{
	Q_OBJECT

public:
	AsyncAction(fuse_req_t req, Controller *thread);

protected:
	virtual void onLoaded(ContentItem *item) = 0;

public slots:
	void loaded(ContentItem *item);

};

#endif // ASYNCACTION_H
