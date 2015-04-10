/*
 * FermenterGPIO.h
 *
 *  Created on: Apr 7, 2015
 *      Author: pete
 */

#ifndef FERMENTERGPIO_H_
#define FERMENTERGPIO_H_

#include <QtCore>

class FermenterGPIO : public QObject {
	Q_OBJECT

public:
	FermenterGPIO(QObject *parent = 0);
	virtual ~FermenterGPIO();
	bool setGPIO(QString);
	bool checkValue(QString);
	QByteArray getValue();
	void setDirection(QByteArray);
	void setValue(QByteArray);

signals:
	void valueChanged(QByteArray);

protected slots:
	void changed(QString);

private:
	QFileSystemWatcher *qfswGPIO;
	QFile *qfDirection;
	QFile *qfValue;
};

#endif /* FERMENTERGPIO_H_ */
