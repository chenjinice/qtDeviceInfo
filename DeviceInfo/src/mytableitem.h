#ifndef MYTABLEITEM_H
#define MYTABLEITEM_H


#define C_SORTROLE   0x100
#define C_IDROLE     0x101
#define C_TCPROLE    0x102
#define C_PORTROLE   0x103
#define C_ERRROLE    0x104

class MyTable;
class MyClient;

#include <QObject>
#include <QTableWidgetItem>


class MyTableItem : public QTableWidgetItem
{
public:
    MyTableItem(MyTable *ui);
    ~MyTableItem();
    void setParam(QString &ip);
    void setBoolIcon(bool flag);
    void setConnectIcon(bool flag);
    uint getErr();
    MyClient *getClient();
    bool operator < (const MyTableItem &other) const;

private:
    MyTable     * m_ui;
    MyClient    * m_client;
    uint          m_sortId;
    uint          m_err;

};

#endif // MYTABLEITEM_H
