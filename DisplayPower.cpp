//
//  DisplayPower.cpp
//  fermentmonitor
//
//  Created by Peter Buelow on 10/22/15.
//
//

#include "DisplayPower.h"

DisplayPower::DisplayPower(QString bright, QObject *parent) : QObject(parent)
{
    iTimeout = MIN_TIMEOUT;
    pTimeout = new QTimer(this);
    pTimeout->setInterval(iTimeout);
    pTimeout->start();
    
    bSleeping = false;
    
    pfBrightnessControl = new QFile(bright);
}

DisplayPower::~DisplayPower()
{
}

void DisplayPower::timeout()
{
    if (!bSleeping) {
        if (pfBrightnessControl->isOpen()) {
            pfBrightnessControl->write("0");
            bSleeping = true;
            pTimeout->stop();
        }
    }
}

void DisplayPower::wakeEvent()
{
    if (bSleeping) {
        if (pfBrightnessControl->isOpen()) {
            pfBrightnessControl->write("100");
            bSleeping = false;
            pTimeout->start(iTimeout);
        }
    }
}

void DisplayPower::setBrightnessLevel(int b)
{
    if (b > 10 || b <= 100) {
        iBrightness = b;
    }
}

void DisplayPower::setTimeout(int t)
{
    if (t > MIN_TIMEOUT && t < MAX_TIMEOUT) {
        iTimeout = t;
    }
}