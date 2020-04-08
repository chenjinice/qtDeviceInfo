#include <QDebug>
#include <QProcess>
#include <QNetworkInterface>
#include <synchapi.h>
#include <QTimer>
#include <QThread>
#include "allip.h"
#include "common.h"


IpData::IpData()
{
    this->port = C_TCPPORT;
    this->cmd = IP_NONE;
}

AllIp::AllIp()
{
    m_loop = false;
    m_count = -1;
    m_thread = new QThread;
    this->moveToThread(m_thread);
    connect(m_thread,&QThread::started,this,&AllIp::run);
    qRegisterMetaType<QList<IpData>>("QList<IpData>");
}

AllIp::~AllIp()
{
    m_loop = false;
    m_thread->quit();
    m_thread->wait();
    delete m_thread;
}

void AllIp::command(QString &cmd, QString &str)
{
    QProcess process;
    process.start(cmd);
    process.waitForStarted();
    if( !process.waitForFinished(3000) ){
        process.kill();
        process.waitForFinished(100);
    }
    str = process.readAll();
}

QStringList AllIp::getSelfIp()
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

uint AllIp::ipToId(QString &ip)
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

void AllIp::setCard(QString &card)
{
    m_mutex.lock();
    m_card = card;
    m_mutex.unlock();
    m_count = -1;

    if(!m_loop){
        m_loop = true;
        m_thread->start();
    }
}

void AllIp::sendIp(QList<IpData> &l)
{
    QList<IpData> ret;
    int count = m_list.count();
    int len = l.count();
    for(int i=0;i<count;i++)m_list[i].cmd = IP_DEL;
    for(int m=0;m<len;m++){
        IpData data = l[m];
        bool flag = false;
        for(int i=0;i<count;i++){
            IpData &mdata = m_list[i];
            if(data.id == mdata.id){
                flag = true;
                mdata.cmd = IP_NONE;
                if(data.mac != mdata.mac){
                   mdata.mac = data.mac;
                   mdata.cmd = IP_UPMAC;
                }
            }
        }
        if(!flag){
            data.cmd = IP_ADD;
            m_list.push_back(data);
        }
    }
    count = m_list.count();
    for(int i=count-1;i>=0;i--){
        IpData &mdata = m_list[i];
//        if(mdata.cmd != IP_NONE){
//            ret.push_back(mdata);
//            if(mdata.cmd == IP_DEL)m_list.removeAt(i);
//            else mdata.cmd = IP_NONE;
//        }
        if(mdata.cmd == IP_ADD){
            ret.push_back(mdata);
            mdata.cmd = IP_NONE;
        }
    }
    if(ret.count() > 0){
        emit sent(ret);
    }
}

void AllIp::getAllIp()
{
    QString str;
    QList<IpData> all;
    m_mutex.lock();
    QString ip = m_card;
    m_mutex.unlock();
    QString cmd = "arp -a -n " + ip;
    int cmp_len = ip.lastIndexOf(".");
    QString pre = ip.left(cmp_len);
    this->command(cmd,str);

    QStringList lines = str.split("\r\n");
    for(int i=0;i<lines.count();i++){
        QString line = lines[i].trimmed().replace(QRegExp("[\\s]+"), " ");
        QStringList array = line.split(" ");
        if(array.count() < 2)continue;
        if(array[0].left(cmp_len) != pre)continue;

        IpData data;
        QStringList nums = array[0].split(".");
        data.ip = array[0];
        data.mac = array[1];
        data.id = AllIp::ipToId(data.ip);
        all.push_back(data);
//        qDebug() << array[0] + " == " << data.id;
    }
    this->sendIp(all);
}

void AllIp::run()
{
    while (m_loop) {
        Sleep(1000);
        if(++m_count%5 == 0)this->getAllIp();
    }
}


