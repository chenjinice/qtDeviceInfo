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
    void setMode(bool flag);
    bool getMode();

private:
    bool             m_mode;
    static Setting * m_instance;

    Setting();
};



#endif // SETTING_H
