#ifndef ASYNCACTION_H
#define ASYNCACTION_H

#include "syncaction.h"

/*abstract*/ class AsyncAction : public SyncAction
{
	Q_OBJECT

public:
	AsyncAction(fuse_req_t req, Controller *thread);
};

#endif // ASYNCACTION_H
