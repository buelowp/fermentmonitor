/*
 * main.cpp
 *
 *  Created on: Jan 24, 2015
 *      Author: pete
 */

#include <QtCore>
#include <QApplication>
#include "FermentMonitor.h"

int main(int argc, char **argv)
{
    QApplication app(argc, argv);
    FermentMonitor monitor;

    monitor.init();
    monitor.show();

    return app.exec();
}
