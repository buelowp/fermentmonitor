/*
 * RestServer.h
 *
 *  This file is part of FermentMonitor.
 *
 *  FermentMonitor is free software: you can redistribute it and/or modify
 *  it under the terms of the GNU General Public License as published by
 *  the Free Software Foundation, either version 3 of the License, or
 *  (at your option) any later version.
 *
 *  FermentMonitor is distributed in the hope that it will be useful,
 *  but WITHOUT ANY WARRANTY; without even the implied warranty of
 *  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
 *  GNU General Public License for more details.
 *
 *  You should have received a copy of the GNU General Public License
 *  along with FermentMonitor.  If not, see <http://www.gnu.org/licenses/>.
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
