#include "mytableitem.h"
#include "myclient.h"
#include "mytable.h"
#include "setting.h"


MyTableItem::MyTableItem(MyTable *ui)
{
    m_ui = ui;
    m_sortId = 0;
    m_err = 0;
    m_client = nullptr;
    this->setTextAlignment(Qt::AlignCenter);
}

MyTableItem::~MyTableItem()
{

}

void MyTableItem::setParam(QString &ip)
{
    if(m_client)return;

    m_sortId = ipToNumber(ip);
    uint16_t port = C_TCPPORT;
    this->setText(ip);
    m_client = new MyClient(ip,port);
    QObject::connect(m_ui,&MyTable::uiConnect,m_client,&MyClient::startThread);
    QObject::connect(m_ui,&MyTable::uiSend,m_client,&MyClient::sendSlot);
    QObject::connect(m_ui,&MyTable::uiQuit,m_client,&MyClient::quitSlot);
    QObject::connect(m_client,&MyClient::toUi,m_ui,&MyTable::showData);
    QObject::connect(m_client,&MyClient::quited,m_ui,&MyTable::clientQuited);
    if(Setting::ins()->getMode()){
        m_client->startThread();
    }
}

MyClient *MyTableItem::getClient()
{
    return m_client;
}

bool MyTableItem::operator <(const MyTableItem &other) const
{
    uint that_id = other.m_sortId;
    return m_sortId < that_id;
}
