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
    qRegisterMetaType<UiCmdData>("UiCmdData");
    qRegisterMetaType<ToUiData>("ToUiData");
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
    UiCmdData data;
    data.all = true;
    data.cmd = UI_QUIT;
    emit uiCmd(data);

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

void MyTable::connectAll()
{
    UiCmdData data;
    data.all = true;
    data.cmd = UI_CONNECT;
    emit uiCmd(data);
    qDebug() << "1111111111";
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
    UiCmdData d;
    this->getSelectedRows(l);
    for(int i=0;i<l.count();i++){
        int row = l[i];
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(row,m_ipIndex));
        d.l.push_back(item->getClient());
    }
    if(s == m_con){d.cmd = UI_CONNECT;}
    else if(s == m_del){d.cmd = UI_QUIT;this->del(l);}
    else if(s == m_tf){d.cmd = UI_SEND;d.str = "ttfcar";}
    else if(s == m_eeprom){d.cmd = UI_SEND;d.str = "tteeprom";}
    else if(s == m_getRtc){d.cmd = UI_SEND;d.str = "rtc-get";}
    else if(s == m_setRtc){d.cmd = UI_SEND;d.str = "rtc-update";}
    else if(s == m_factory){d.cmd = UI_SEND;d.str = "refactory force";}
    emit uiCmd(d);
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

void MyTable::showData(ToUiData d)
{
    MyClient *s = qobject_cast<MyClient *>(sender());
    int count = this->rowCount();
    int row = -1;
    for(int i=0;i<count;i++){
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(i,m_ipIndex));
        if(item->getClient() == s){
            if(d.stateChanged)item->setConnectIcon(d.state);
            row = i;
        }
    }
    if(row == -1)return;

    int l_count = d.l.count();
    for(int m=0;m<l_count;m++){
        const ItemData &idata = d.l[m];
        int col = idata.column;
        QString text;
        MyTableItem *item = dynamic_cast<MyTableItem *>(this->item(row,col));
        if(item == nullptr)continue;
        if(idata.hasFlag)item->setBoolIcon(idata.flag);
        uint err = item->getErr();
        if(!idata.text.isEmpty())text+=idata.text;
        if((err > 0) && (col!=m_getRtcIndex) && (col!=m_setRtcIndex) ){
            text += "┏" + QString::number(err) + "┛";
        }
        item->setText(text);
    }
}

void MyTable::clientQuited()
{
    MyClient *s = qobject_cast<MyClient *>(sender());
    delete s;
}


