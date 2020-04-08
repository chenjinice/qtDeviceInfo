#ifndef MYTABLE_H
#define MYTABLE_H


#include <QTableWidget>
#include <QMap>
#include "myclient.h"
#include "common.h"
#include "mytableitem.h"


class MyTable : public QTableWidget
{
    Q_OBJECT
public:
    MyTable();
    ~MyTable();
    void addIp(QString ip);
    void clearResult();
    void sortByIp();

private:
    int                    m_ipIndex;
    int                    m_getRtcIndex;
    int                    m_setRtcIndex;
    QMenu                * m_menu;
    QAction              * m_con;
    QAction              * m_tf;
    QAction              * m_eeprom;
    QAction              * m_getRtc;
    QAction              * m_setRtc;
    QAction              * m_del;
    QAction              * m_factory;
    QStringList            m_header;
    QList<MyClient *>      m_clients;

    void createMenu();
    void showContextMenu(const QPoint &pos);
    void setBoolIcon(QTableWidgetItem *item,bool flag);
    void setConnectIcon(QTableWidgetItem *item,bool flag);
    void getSelectedRows(QList<int> &l);
    void ActionClicked();
    void del(QList<int> &l);

public slots:
    void showData(UiData data);
    void clientQuited();

signals:
    void uiSend(const char *cmd,QList<MyClient *> l);
    void uiCmd(QList<MyClient *> l,UICMD cmd);
    void uiConnect(QList<CInfo> l);
    void uiQuit(QList<CInfo> l);
};

#endif // MYTABLE_H
