#ifndef ALLIP_H
#define ALLIP_H


#include <QObject>
#include <QMutex>
#include <QWidget>

#define C_PORT  12301

enum IpOperate{
    IP_NONE = 0,
    IP_ADD,
    IP_DEL,
    IP_UPMAC
};

class IpData
{
public:
    IpData();
    uint      id;
    QString   ip;
    QString   mac;
    int       port;
    IpOperate cmd;
};


class AllIp : public QObject
{
    Q_OBJECT
public:
    AllIp();
    ~AllIp();
    void command(QString &cmd, QString &str);
    static QStringList getSelfIp();
    static uint ipToId(QString &ip);
    void setCard(QString &card);

private:
    int            m_count;
    bool           m_loop;
    QMutex         m_mutex;
    QString        m_card;
    QThread       *m_thread;
    QList<IpData>  m_list;

    void sendIp(QList<IpData> &l);
    void getAllIp();
    void run();


signals:
    void sent(QList<IpData> l);
};


#endif // ALLIP_H
