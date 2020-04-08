#include <QThread>
#include <QUdpSocket>
#include <QDebug>
#include "udpthread.h"
#include "mainwindow.h"
#include "setting.h"

UdpThread  * UdpThread::m_instance = nullptr;


UdpThread *UdpThread::ins()
{
    if(m_instance == nullptr){
        m_instance = new UdpThread();
    }
    return  m_instance;
}

void UdpThread::startThread(QString &ip)
{
    if(!m_thread->isRunning()){
        m_ip = ip;
        m_thread->start();
    }
}

UdpThread::UdpThread()
{
    m_udp = nullptr;
    m_ip  = "127.0.0.1";
    m_thread = new QThread;
    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&UdpThread::threadRun);
}

UdpThread::~UdpThread()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
    if(m_udp){
        m_udp->close();
        delete m_udp;
    }
}

void UdpThread::setIp(QString ip)
{
    Setting::ins()->setCard(ip);
    m_ip = ip;
    this->bindIp();
}

void UdpThread::threadRun()
{
    m_udp = new QUdpSocket;
    this->bindIp();
    connect(m_udp, &QUdpSocket::readyRead, this, &UdpThread::readData);
}

void UdpThread::bindIp()
{
    if(!m_udp)return;
    m_udp->close();
    bool flag = m_udp->bind(QHostAddress(m_ip), C_UDPPORT);
    emit bindState(flag);
}

void UdpThread::readData()
{
    QByteArray array;
    QHostAddress address;
    int len = static_cast<int>(m_udp->pendingDatagramSize());
    array.resize(len);
    m_udp->readDatagram(array.data(), array.size(),&address);

    QString ip = address.toString();
    if(m_list.indexOf(ip) == -1){
        m_list.append(ip);
        emit getIp(ip);
    }
}
