#ifndef COMMON_H
#define COMMON_H

class MyClient;

#include <QString>
#include <QStringList>


#define C_TCPPORT      12301
#define C_UDPPORT      12302
#define C_RETIME       5000     // 重连定时器间隔 ,毫秒
#define C_MOTIME       5000     // 监控定时器间隔 ,毫秒
#define C_MAXTIME      5        // 多长时间没收到数据就断开 ,秒

#define C_OK        "ok"
// 显示项
#define CI_IP       "ip"
#define CI_USB      "usb-com"
#define CI_GPS      "gps"
#define CI_RS485    "rs485"
#define CI_CAN      "can"
#define CI_4G       "4G"
#define CI_RJ45     "rj45"
#define CI_WIFI     "wifi"
#define CI_TF       "tf"
#define CI_EEPROM   "eeprom"
#define CI_RTCGET   "rtc_get"
#define CI_RTCSET   "rtc_set"
#define CI_TMP1     "主板温度"
#define CI_TMP2     "CPU温度"
#define CI_TIME     "设备时间"

enum UICMD{
    UI_NONE,
    UI_CONNECT,
    UI_DISCONNECT,
    UI_SEND,
    UI_DELETE,
};


uint ipToNumber(QString &ip);
QStringList getSelfIp();
QStringList getTestItems();


class UiCmdData{
public:
    UiCmdData(){
        all = false;
        cmd = UI_NONE;
        str = nullptr;
    }
    bool               all;
    UICMD              cmd;
    const char       * str;
    QList<MyClient *>  l;
};


class ItemData{
public:
    ItemData(){
        column = -1;
        hasFlag = false;
        flag    = false;
    }
    int     column;
    bool    hasFlag;
    bool    flag;
    QString text;
};

class ToUiData{
public:
    ToUiData(){
        stateChanged = false;
        state = false;
    }
    bool  stateChanged;
    bool  state;
    QList<ItemData> l;
};

#endif // COMMON_H
