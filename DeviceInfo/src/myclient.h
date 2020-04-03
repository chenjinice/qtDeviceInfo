#ifndef MYCLIENT_H
#define MYCLIENT_H


class QThread;
class QTcpSocket;
class QTimer;
class MyTable;

#include <QObject>
#include <QTcpSocket>
#include <QThread>
#include <QMap>

#define C_PORT      12301
#define C_OK        "ok"

#define CI_IP       "ip"
#define CI_PORT     "port"
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


class UiData{
public:
    UiData();
    bool              stateChanged;
    bool              state;
    QMap<int,bool>    b;
    QMap<int,QString> text;
};

class CInfo
{
public:
    uint id;
    int  port;
};

class MyClient : public QObject
{
    Q_OBJECT
public:
    MyClient(uint id,QString ip, quint16 port, MyTable *ui);
    ~MyClient();
    static QStringList testItems();
    uint id();
    int  port();
    void startThread();

private:
    static QStringList m_items;
    bool           m_connected;
    uint           m_id;
    QString        m_ip;
    int            m_port;
    QTimer       * m_timer;
    QThread      * m_thread;
    QTcpSocket   * m_socket;
    MyTable      * m_ui;

    void threadRun();
    void reConnect();
    void sockConnected();
    void sockDisconnected();
    void quitConnection();
    void send(const char * cmd);
    void connectSlot(QList<CInfo> l);
    void sendSlot(const char *cmd,QList<CInfo> l);
    void quitSlot(QList<CInfo> l);
    void sockReadData();
    void parseData(QString &str);
    bool parsePre1(QString &str, int len,UiData &data);
    bool parsePre2(QString &str, int len,UiData &data);
    bool checkTime(QString &str);
    void saveLog(QString &str);

signals:
    void toUi(UiData data);
    void quited();

};

#endif // MYCLIENT_H
