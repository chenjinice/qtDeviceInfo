#ifndef SETTING_H
#define SETTING_H


#include <QSettings>

class Setting : public QSettings
{
public:
    static Setting *ins();
    void setCard(QString str);
    QString getCard();
    void setEditIp(QString ip);
    QString getEditIp();
    void setEditPort(QString port);
    QString getEditPort();

private:
    Setting();
    static Setting *m_instance;
};



#endif // SETTING_H
