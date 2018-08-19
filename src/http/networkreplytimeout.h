#ifndef NETWORKREPLYTIMEOUT_H
#define NETWORKREPLYTIMEOUT_H

#include <QObject>
#include <QBasicTimer>
#include <QNetworkReply>

class NetworkReplyTimeout : public QObject
{
	Q_OBJECT
public:
	explicit NetworkReplyTimeout(QNetworkReply *reply, const int timeoutMs);

	inline static void set(QNetworkReply *reply, const int timeoutMs){
		new NetworkReplyTimeout(reply, timeoutMs);
	}

protected:
	void timerEvent(QTimerEvent *ev);
	void progressEvent(qint64 bytesReceived, qint64 bytesTotal);

private:
	QBasicTimer _timer;
	const int _timeoutMs;

};

#endif // NETWORKREPLYTIMEOUT_H
