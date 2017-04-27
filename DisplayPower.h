//
//  DisplayPower.hpp
//  fermentmonitor
//
//  Created by Peter Buelow on 10/22/15.
//
//

#ifndef DisplayPower_hpp
#define DisplayPower_hpp

#define MIN_TIMEOUT     (1000 * 60)
#define MAX_TIMEOUT     (MIN_TIMEOUT * 10)

#include <QtCore>

class DisplayPower : public QObject
{
    Q_OBJECT
public:
    DisplayPower(QString, QObject *parent = 0);
    virtual ~DisplayPower();
    
    void setBrightnessLevel(int);
    int getBrightnessLevel() { return iBrightness; }
    void setTimeout(int);
    int getTimeout() { return iTimeout; }
    
public slots:
    void timeout();
    void wakeEvent();
    
private:
    QTimer *pTimeout;
    QFile *pfBrightnessControl;
    bool bSleeping;
    int iBrightness;
    int iTimeout;
};

#endif /* DisplayPower_hpp */
