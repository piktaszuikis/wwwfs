#include <QDebug>
#include <QTimerEvent>
#include "networkreplytimeout.h"

NetworkReplyTimeout::NetworkReplyTimeout(QNetworkReply *reply, const int timeoutMs)
	: QObject(reply), _timeoutMs(timeoutMs)
{
	Q_ASSERT(reply);

	connect(reply, &QNetworkReply::downloadProgress, this, &NetworkReplyTimeout::progressEvent);

	if(reply && reply->isRunning())
		_timer.start(timeoutMs, this);
}

void NetworkReplyTimeout::timerEvent(QTimerEvent *ev)
{
	if(!_timer.isActive() || ev->timerId() != _timer.timerId())
		return;

	auto reply = static_cast<QNetworkReply*>(parent());
	if(reply->isRunning()){
		qWarning() << "Cancelling request, because of timeout" << reply->url();
		reply->close();
	}

	_timer.stop();
}

void NetworkReplyTimeout::progressEvent(qint64, qint64)
{
	_timer.start(_timeoutMs, this);
}
