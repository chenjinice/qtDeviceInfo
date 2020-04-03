#include "setting.h"

#define CS_CARD     "card"
#define CS_EDITIP   "editIp"
#define CS_EDITPORT "editPort"

Setting *Setting::m_instance = nullptr;

Setting *Setting::ins()
{
    if(m_instance == nullptr){
        m_instance = new Setting;
    }
    return m_instance;
}

Setting::Setting() : QSettings("CIDI","device_info")
{

}

void Setting::setCard(QString str)
{
    this->setValue(CS_CARD,str);
}

QString Setting::getCard()
{
    return this->value(CS_CARD).toString();
}

void Setting::setEditIp(QString ip)
{
    this->setValue(CS_EDITIP,ip);
}

QString Setting::getEditIp()
{
    return this->value(CS_EDITIP).toString();
}

void Setting::setEditPort(QString port)
{
    this->setValue(CS_EDITPORT,port);
}

QString Setting::getEditPort()
{
    return this->value(CS_EDITPORT).toString();
}



