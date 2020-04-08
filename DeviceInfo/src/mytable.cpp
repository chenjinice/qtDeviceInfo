#include <QHeaderView>
#include <QtAlgorithms>
#include <QMenu>
#include <QAction>
#include <QIcon>
#include <QDebug>
#include "mytable.h"
#include "mytableitem.h"
#include "myclient.h"


MyTable::MyTable()
{
    m_header = MyClient::testItems();
    m_ipIndex = m_header.indexOf(CI_IP);
    m_getRtcIndex = m_header.indexOf(CI_RTCGET);
    m_setRtcIndex = m_header.indexOf(CI_RTCSET);
    int cols = m_header.count();
    this->createMenu();
    this->setContextMenuPolicy(Qt::CustomContextMenu);
    this->setAlternatingRowColors(true);
    this->setColumnCount(cols);
    this->setRowCount(0);
    this->setHorizontalHeaderLabels(m_header);
    this->horizontalHeader()->setStyleSheet("QHeaderView{border:0;border-bottom:1px solid #000;}");
//    this->horizontalHeader()->setStretchLastSection(true);
    this ->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
    this->horizontalHeader()->setSectionResizeMode(m_ipIndex, QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(m_getRtcIndex, QHeaderView::ResizeToContents);
    this->horizontalHeader()->setSectionResizeMode(m_setRtcIndex, QHeaderView::ResizeToContents);
    this->setEditTriggers(QAbstractItemView::NoEditTriggers);

    connect(this,&MyTable::customContextMenuRequested,this,&MyTable::showContextMenu);
}

MyTable::~MyTable()
{
    QList<CInfo> clients;
    for(int i=0;i<this->rowCount();i++){
        CInfo c;
        QTableWidgetItem *item = this->item(i,m_ipIndex);
        c.id = item->data(C_SORTROLE).toUInt();
        c.port = item->data(C_PORTROLE).toInt();
        clients.push_back(c);
    }
    emit uiQuit(clients);
    int rows = this->rowCount();
    int cols = this->columnCount();
    for (int i=0;i<rows;i++) {
        for(int m=0;m<cols;m++){
            QTableWidgetItem *item = this->item(i,m);
            delete item;
        }
    }
    this->clear();
}

void MyTable::addIp(QString ip)
{
    int col = m_header.count();
    int row = this->rowCount();
    this->insertRow(row);
    for(int m=0;m<col;m++){
        MyTableItem *item = new MyTableItem(this);
        if(m == m_ipIndex){
           item->setParam(ip);
        }
        this->setItem(row,m,item);
    }
}

void MyTable::clearResult()
{
    int rows = this->rowCount();
    int cols = this->columnCount();
    for (int i=0;i<rows;i++) {
        for(int m=0;m<cols;m++){
            if(m == m_ipIndex)continue;
            QTableWidgetItem *item = this->item(i,m);
            item->setIcon(QIcon());
            item->setText("");
        }
    }
}

void MyTable::sortByIp()
{
    this->sortByColumn(m_ipIndex,Qt::AscendingOrder);
}

void MyTable::createMenu()
{
    m_menu = new QMenu(this);
    m_con = new QAction("连接");
    m_tf  = new QAction("测tf");
    m_eeprom = new QAction("测eeprom");
    m_getRtc = new QAction("获取RTC");
    m_setRtc = new QAction("更新RTC");
    m_del = new QAction("删除");
    m_factory = new QAction("恢复出厂设置");
    m_menu->addAction(m_tf);
    m_menu->addAction(m_eeprom);
    m_menu->addAction(m_getRtc);
    m_menu->addAction(m_setRtc);
    m_menu->addSeparator();
    m_menu->addAction(m_con);
    m_menu->addAction(m_del);
    m_menu->addSeparator();
//    m_menu->addAction(m_factory);
//    m_factory->setEnabled(false);
    connect(m_tf,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_eeprom,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_getRtc,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_setRtc,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_con,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_del,&QAction::triggered,this,&MyTable::ActionClicked);
    connect(m_factory,&QAction::triggered,this,&MyTable::ActionClicked);
}

void MyTable::showContextMenu(const QPoint &pos)
{
    qDebug() << pos;
    m_menu->exec(QCursor::pos());
}

void MyTable::setBoolIcon(QTableWidgetItem *item, bool flag)
{
    static QIcon yes(":/icon/image/yes.png");
    static QIcon no(":/icon/image/no.png");
    if(flag)item->setIcon(yes);
    else item->setIcon(no);
}

void MyTable::setConnectIcon(QTableWidgetItem *item, bool flag)
{
    static QIcon c_yes(":/icon/image/connect_yes.png");
    static QIcon c_no(":/icon/image/connect_no.png");
    if(flag)item->setIcon(c_yes);
    else item->setIcon(c_no);
}

void MyTable::getSelectedRows(QList<int> &l)
{
    const QList<QTableWidgetItem*> &items= this->selectedItems();
    int count= items.count();
    for(int i=0;i<count;i++){
        int row = this->row(items[i]);
        if(l.indexOf(row) == -1)l.push_back(row);
    }
    std::sort(l.begin(),l.end());
}

void MyTable::ActionClicked()
{
    QAction *s = qobject_cast<QAction *>(sender());
    QList<int> l;
    QList<MyClient *> clients;
    this->getSelectedRows(l);
    for(int i=0;i<l.count();i++){
        int row = l[i];
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(row,m_ipIndex));
        clients.push_back(item->getClient());
    }
    if(s == m_con){emit uiCmd(clients,UI_CONNECT);}
    else if(s == m_del){emit uiCmd(clients,UI_QUIT);this->del(l);}
    else if(s == m_tf){emit uiSend("ttfcar",clients);}
    else if(s == m_eeprom){emit uiSend("tteeprom",clients);}
    else if(s == m_getRtc){emit uiSend("rtc-get",clients);}
    else if(s == m_setRtc){emit uiSend("rtc-update",clients);}
    else if(s == m_factory){emit uiSend("refactory force",clients);}
}

void MyTable::del(QList<int> &l)
{
    int row_count = l.count();
    int col_count = m_header.count();
    for(int i=row_count-1;i>=0;i--){
        int row = l[i];
        for(int j=0;j<col_count;j++){
            QTableWidgetItem *item = this->item(row,j);
            if(item)delete item;
        }
        this->removeRow(row);
    }
}

void MyTable::showData(UiData data)
{
    MyClient *s = qobject_cast<MyClient *>(sender());
//    uint c_id = s->id();
    uint c_id = 111;
    int  c_port = s->port();
    int count = this->rowCount();
    int row = -1;
    for(int i=0;i<count;i++){
        QTableWidgetItem *item = this->item(i,m_ipIndex);
        uint id = item->data(C_SORTROLE).toUInt();
        int port = item->data(C_PORTROLE).toInt();
        if(c_id != id)continue;
        if(c_port != port)continue;
        row = i;
    }
    if(row == -1)return;
    QMapIterator<int,bool> it(data.b);
    QMapIterator<int,QString> it2(data.text);
    if(data.stateChanged)this->setConnectIcon(this->item(row,m_ipIndex),data.state);
    while(it.hasNext()){
        int col = it.next().key();
        bool value = it.value();
        QTableWidgetItem *item = this->item(row,col);
        this->setBoolIcon(item,value);
        if(!value){
            int err = item->data(C_ERRROLE).toInt()+1;
            item->setData(C_ERRROLE,err);
            if((col!=m_getRtcIndex) && (col!=m_setRtcIndex))item->setData(0,err);
        }
    }
    while(it2.hasNext()){
        this->item(row,it2.next().key())->setText(it2.value());
    }
}

void MyTable::clientQuited()
{
    MyClient *s = qobject_cast<MyClient *>(sender());
    m_clients.removeOne(s);
    delete s;
}




