#ifndef UDPTHREAD_H
#define UDPTHREAD_H

class QThread;
class QUdpSocket;
class MainWindow;

#include <QObject>
#include "common.h"

class UdpThread : public QObject
{
    Q_OBJECT
public:
    static UdpThread *ins();
    void startThread(QString &ip);
    ~UdpThread();

private:
    static UdpThread * m_instance;
    QString            m_ip;
    QThread          * m_thread;
    QUdpSocket       * m_udp;
    QList<QString>     m_list;

    UdpThread();
    void threadRun();
    void bindIp();
    void readData();

public slots:
    void setIp(QString ip);

signals:
    void bindState(bool flag);
    void getIp(QString ip);
};

#endif // UDPTHREAD_H
