/*
 * RestServer.h
 *
 *  Created on: Jan 24, 2015
 *      Author: pete
 */

#ifndef RESTSERVER_H_
#define RESTSERVER_H_

#include <QtNetwork>

class RestServer : public QTcpServer {
	Q_OBJECT
public:
	RestServer(quint16 port, QObject *parent = 0) : QTcpServer(parent)
	{
		listen(QHostAddress::Any, port);
	}
	virtual ~RestServer();

	void incomingConnect(int);
	void pause();
	void resume();

private slots:
	void readClient();
	void discardClient();

private:
	void decodeGetRequest(QStringList&);
};

#endif /* RESTSERVER_H_ */
