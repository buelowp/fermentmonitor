/*
 * RestServer.cpp
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

#include "RestServer.h"

RestServer::~RestServer() {
	// TODO Auto-generated destructor stub
}

void RestServer::incomingConnection(int socket)
{
     QTcpSocket* s = new QTcpSocket(this);
     connect(s, SIGNAL(readyRead()), this, SLOT(readClient()));
     connect(s, SIGNAL(disconnected()), this, SLOT(discardClient()));
     connect(this, SIGNAL(newGetRequest(QVector<QStringList>)), this, SLOT(handleGetRequest(QVector<QStringList>)));
     s->setSocketDescriptor(socket);
}

int RestServer::decodeRequestType(QString rqst)
{
    if (rqst == "GET") {
    	return RS_HTTP_GET;
    }
    if (rqst == "POST") {
    	return RS_HTTP_POST;
    }
    if (rqst == "POST") {
    	return RS_HTTP_POST;
    }
    if (rqst == "DELETE") {
    	return RS_HTTP_DELETE;
    }
    if (rqst == "HEAD") {
    	return RS_HTTP_HEAD;
    }
    return -1;
}

void RestServer::handleGetRequest(QVector<QStringList> clientRequest)
{
	QTcpSocket *socket = (QTcpSocket*)sender();
	QStringList newRequest = clientRequest[0];
	QString api = newRequest[1];
	int pos;

	if (api.contains("/temperature")) {
		if ((pos = api.lastIndexOf('/')) > 0) {
			pos++;
			QString name = api.mid(pos, (api.size() - pos));
			if (probes.contains(name)) {
				sendTempResponse(probes.value(name));
			}
			else {
				sendTempResponse((double)0.0);
			}
		}
	}
}

void RestServer::sendTempResponse(double temp)
{
	QTcpSocket *socket = (QTcpSocket*)sender();

	QTextStream os(socket);
	QStringList response;
	response << "HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=\"utf-8\"\r\n\r\n";
	response << "Temperature: " << temp << "\n";

    os.setAutoDetectUnicode(true);
    os << response;
}

void RestServer::readClient()
{
	int requestType = 0;
	bool typeDecoded = false;
	QVector<QStringList> clientRequest;

    QTcpSocket* socket = (QTcpSocket*)sender();
    while (socket->canReadLine()) {
        QStringList tokens = QString(socket->readLine()).split(QRegExp("[ \r\n][ \r\n]*"));
        if (!typeDecoded) {
        	requestType = decodeRequestType(tokens[0]);
        }
        clientRequest.push_back(tokens);
    }
    if (requestType = RS_HTTP_GET) {
    	handleGetRequest(clientRequest);
    }
}

void RestServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}
