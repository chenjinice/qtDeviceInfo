#include "setting.h"

#define CS_MODE     "mode"
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
    m_mode = this->value(CS_MODE).toBool();
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

void Setting::setMode(bool flag)
{
    m_mode = flag;
    this->setValue(CS_MODE,flag);
}

bool Setting::getMode()
{
    return m_mode;
}




