#include "setting.h"


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
    this->setValue(CS_MODE,true);
}

void Setting::setCard(QString str)
{
    this->setValue(CS_CARD,str);
}

QString Setting::getCard()
{
    return this->value(CS_CARD).toString();
}

void Setting::setMode(bool flag)
{
    this->setValue(CS_MODE,flag);
}

bool Setting::getMode()
{
    return this->value(CS_MODE).toBool();
}

void Setting::setAutoSort(bool flag)
{
    this->setValue(CS_SORT,flag);
}

bool Setting::getAutoSort()
{
    return this->value(CS_SORT).toBool();
}




