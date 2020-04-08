#ifndef COMMON_H
#define COMMON_H

class MyClient;

#include <QString>
#include <QStringList>

#define C_TCPPORT      12301
#define C_UDPPORT      12302
#define C_RETIME       5000

enum UICMD{
    UI_NONE,
    UI_CONNECT,
    UI_SEND,
    UI_QUIT,
};


uint ipToNumber(QString &ip);
QStringList getSelfIp();


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
