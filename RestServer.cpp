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

void RestServer::acceptConnection()
{
	QTcpSocket *socket = nextPendingConnection();
	if (socket) {
	     connect(socket, SIGNAL(readyRead()), this, SLOT(readClient()));
	     connect(socket, SIGNAL(disconnected()), this, SLOT(discardClient()));
	     connect(this, SIGNAL(newGetRequest(QVector<QStringList>)), this, SLOT(handleGetRequest(QVector<QStringList>)));
	}
}

void RestServer::handleGetRequest(QVector<QStringList> clientRequest)
{
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
    os.setAutoDetectUnicode(true);
    os << "HTTP/1.0 200 OK\r\nContent-Type: text/html; charset=\"utf-8\"\r\nTemperature: " << temp << "\r\n\r\n";
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
    if (requestType == RS_HTTP_GET) {
    	handleGetRequest(clientRequest);
    }
}

void RestServer::discardClient()
{
    QTcpSocket* socket = (QTcpSocket*)sender();
    socket->deleteLater();
}

void RestServer::setBubbleCount(QString n, int i)
{
	if (bubbleCount.contains(n)) {
		bubbleCount[n] = i;
	}
}

void RestServer::setBubblesPerMin(QString n, int i)
{
	if (bubblesPerMin.contains(n)) {
		bubblesPerMin[n] = i;
	}
}

void RestServer::setFermentState(QString n, bool s)
{
	if (fermentState.contains(n)) {
		fermentState[n] = s;
	}
}

void RestServer::setTemp(QString n, double t)
{
	if (probes.contains(n)) {
		probes[n] = t;
	}
}
