#include <QDebug>
#include <QTimer>
#include <QTcpSocket>
#include <QDir>
#include <QDateTime>
#include <QFile>
#include <synchapi.h>
#include <QApplication>
#include "myclient.h"
#include "mytable.h"


QStringList MyClient::m_items;


UiData::UiData()
{
    this->stateChanged = false;
    this->state = false;
}

MyClient::MyClient(uint id, QString ip, quint16 port, MyTable *ui)
{
    qRegisterMetaType<UiData>("UiData");
    qRegisterMetaType<QList<CInfo>>("QList<CInfo>");
    m_connected = false;
    m_id = id;
    m_ip = ip;
    m_port = port;
    m_ui = ui;
    m_thread = new QThread;
    m_socket = nullptr;
    m_timer  = nullptr;

    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&MyClient::threadRun);
    connect(m_ui,&MyTable::uiConnect,this,&MyClient::startThread);
    connect(m_ui,&MyTable::uiSend,this,&MyClient::sendSlot);
    connect(m_ui,&MyTable::uiQuit,this,&MyClient::quitSlot);
//    m_thread->start();
}

MyClient::~MyClient()
{
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

QStringList MyClient::testItems()
{
    if(m_items.count() == 0){
        m_items << CI_IP << CI_PORT << CI_USB << CI_GPS << CI_RS485 << CI_CAN << CI_4G << CI_RJ45 << CI_WIFI
                << CI_TF << CI_EEPROM << CI_RTCGET << CI_RTCSET << CI_TMP1 << CI_TMP2 << CI_TIME;
    }
    return m_items;
}

uint MyClient::id()
{
    return m_id;
}

int MyClient::port()
{
    return m_port;
}

void MyClient::startThread()
{
    m_thread->start();
}

void MyClient::threadRun()
{
    m_timer = new QTimer;
    m_socket = new QTcpSocket;
    m_timer->setInterval(10000);
    connect(m_timer,&QTimer::timeout,this,&MyClient::reConnect);
    connect(m_socket,&QTcpSocket::connected,this,&MyClient::sockConnected);
    connect(m_socket,&QTcpSocket::disconnected,this,&MyClient::sockDisconnected);
    connect(m_socket,&QTcpSocket::readyRead,this,&MyClient::sockReadData);
    m_socket->connectToHost(m_ip,m_port,QTcpSocket::ReadWrite);
    m_socket->waitForConnected(500);
    if(!m_connected){
        m_timer->start();
    }
}

void MyClient::reConnect()
{
    if(!m_connected){
        m_socket->disconnectFromHost();
        m_socket->abort();
        m_socket->connectToHost(m_ip,m_port,QTcpSocket::ReadWrite);
        m_socket->waitForConnected(500);
    }
}

void MyClient::sockConnected()
{
    m_connected = true;
    UiData data;
    data.stateChanged = true;
    data.state = true;
    emit toUi(data);

    if(m_timer)m_timer->stop();
    this->send("ttuv2x");
}

void MyClient::sockDisconnected()
{
    m_connected = false;
    UiData data;
    data.stateChanged = true;
    data.state = false;
    emit toUi(data);

    if(m_timer)m_timer->start();
}

void MyClient::quitConnection()
{
    if(m_timer){
        m_timer->stop();
        delete m_timer;
        m_timer = nullptr;
    }
    if(m_socket){
        m_socket->disconnectFromHost();
        m_socket->abort();
        delete m_socket;
        m_socket = nullptr;
    }
}

void MyClient::send(const char *cmd)
{
//    qDebug() << "send";
    if(m_connected)m_socket->write(cmd,strlen(cmd));
}

void MyClient::connectSlot(QList<CInfo> l)
{
    bool flag = false;
    int count = l.count();
    for(int i=0;i<count;i++){
        if(l[i].id != m_id)continue;
        if(l[i].port != m_port)continue;
        flag = true;
    }
    if(flag)this->startThread();
}

void MyClient::sendSlot(const char *cmd, QList<CInfo> l)
{
    bool flag = false;
    int count = l.count();
    for(int i=0;i<count;i++){
        if(l[i].id != m_id)continue;
        if(l[i].port != m_port)continue;
        flag = true;
    }
    if(flag)this->send(cmd);
}

void MyClient::quitSlot(QList<CInfo> l)
{
    bool flag = false;
    int count = l.count();
    for(int i=0;i<count;i++){
        if(l[i].id != m_id)continue;
        if(l[i].port != m_port)continue;
        flag = true;
    }
    if(flag){
        this->quitConnection();
        emit quited();
    }
}

void MyClient::sockReadData()
{
    QByteArray array = m_socket->readAll();
    QString str = array;
    this->saveLog(str);
    this->parseData(str);
}

void MyClient::parseData(QString &str)
{
    QString pre1 = "-----";
    QString pre2 = "=================";
    UiData data;
    QStringList lines = str.remove("\r").split("\n");
    int line_count = lines.count();
    bool flag = false;
    for(int i=0;i<line_count;i++){
        if(lines[i].indexOf(pre1) == 0){
            flag |= this->parsePre1(lines[i],pre1.length(),data);
        }else if(lines[i].indexOf(pre2) == 0){
            flag |= this->parsePre2(lines[i],pre2.length(),data);
        }
    }
    if(flag)emit toUi(data);
//    qDebug() << "-------------------";
//    QMapIterator<int,bool> it(data.b);
//    QMapIterator<int,QString> it2(data.text);
//    while(it.hasNext()){
//        qDebug() << it.next().key() << " -- " << it.value();
//    }
//    while(it2.hasNext()){
//        qDebug() << it2.next().key() << " --- " << it2.value();
//    }

}

bool MyClient::parsePre1(QString &str, int len, UiData &data)
{
    bool ret = false;
    bool value = false;
    QStringList array = str.mid(len).replace(QRegExp("[\\s]+")," ").split(" ");
    if(array.length() < 2)return ret;
    QString result = array[0].mid(0,array[0].indexOf("."));
    QString item   = array[1].mid(0,array[1].indexOf("."));
    int index = m_items.indexOf(item);
    if(index == -1)return ret;
    if(result.compare(C_OK,Qt::CaseInsensitive) == 0)value = true;
    data.b[index] = value;

    if((item == CI_RTCGET) || (item == CI_RTCSET)){
        int count = array.length()-2;
        QString text;
        for(int i=2;i<count;i++){text += array[i];if(i < count -1)text += " ";}
        data.text[index] = text;
        bool flag = false;
        if(count >= 7){
            QString time_str = array[6]+" "+array[3]+" "+array[4]+" "+array[5];
            flag = this->checkTime(time_str);
        }
        data.b[index] = flag;
    }
    return true;
}

bool MyClient::parsePre2(QString &str, int len, UiData &data)
{
    int index;
    QString text;
    bool ret = false;
    QStringList array = str.mid(len).split(" ");
    if(array.length() < 2)return ret;

    index = m_items.indexOf(CI_TIME);
    if(index != -1){
        if(array.length() > 2)text += array[2];
        if(array.length() > 3)text +=" " + array[3];
        data.text.insert(index,text);
        ret = true;
    }
    index = m_items.indexOf(CI_TMP1);
    if(index != -1){
        double temp1 = array[0].toInt()/1000.0;
        text = QString::number(temp1,'f',3);
        data.text.insert(index,text);
        ret = true;
    }
    index = m_items.indexOf(CI_TMP2);
    if(index != -1){
        double temp2 = array[1].toInt()/1000.0;
        text = QString::number(temp2,'f',3);
        data.text.insert(index,text);
        ret = true;
    }
    return true;
}

bool MyClient::checkTime(QString &str)
{
    bool flag = false;
    QLocale locale = QLocale::English;
    QDateTime d = locale.toDateTime(str,"yyyy MMM dd hh:mm:ss");
    QDateTime current = QDateTime::currentDateTime();
    qint64 seconds = qAbs(d.secsTo(current));
    qDebug() << "seconds = " << seconds;
    if(seconds < 120)flag = true;
    return flag;
}

void MyClient::saveLog(QString &str)
{
    QString dirct = "log";
    QDir dir;
    if(!dir.exists(dirct))dir.mkdir(dirct);
    QString path = dirct + "/" + QDateTime::currentDateTime().toString("yyyyMMdd__")+m_ip+"_"+QString::number(m_port)+".log";
    QFile file(path);
    QString log ;
    bool ret = file.open(QIODevice::Append);
    if(!ret)return;
    log += QDateTime::currentDateTime().toString("[yyyy.MM.dd hh:mm:ss]  : \r\n");
    log += str +"\r\n";
    file.write(log.toUtf8());
    file.close();
}
