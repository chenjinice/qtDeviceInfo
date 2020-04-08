#include <QNetworkInterface>
#include <QDebug>
#include "common.h"


uint ipToNumber(QString &ip)
{

    uint id = 0;
    QStringList nums = ip.split(".");
    if(nums.count() == 4){
        id = nums[0].toUInt()*256*256*256 + nums[1].toUInt()*256*256 + nums[2].toUInt()*256 + nums[3].toUInt();
    }else{
        qDebug() << "ipToId error : count != 4";
    }
    return id;
}


QStringList getSelfIp()
{
    QStringList value;
    QList<QHostAddress> l = QNetworkInterface::allAddresses();
    int n = l.size();
    for (int i = 0; i < n; i++){
        if (l.at(i) != QHostAddress::LocalHost && l.at(i).toIPv4Address()) {
            value <<  l.at(i).toString();
        }
    }
    return value;
}
