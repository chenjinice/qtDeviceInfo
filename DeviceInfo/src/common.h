#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QStringList>

#define C_TCPPORT      12301
#define C_UDPPORT      12302

enum UICMD{
    UI_CONNECT,
    UI_QUIT
};

uint ipToNumber(QString &ip);
QStringList getSelfIp();



#endif // COMMON_H
