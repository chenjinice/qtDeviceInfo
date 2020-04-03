#include <QDebug>
#include <QLabel>
#include <QTextEdit>
#include <QPushButton>
#include <QTableWidget>
#include <QBoxLayout>
#include <QHeaderView>
#include <QStringList>
#include "devicewindow.h"


DeviceWindow::DeviceWindow(QWidget *parent) : QWidget(parent)
{
    m_items << "usb-com"<< "gps" << "rs485" << "can" << "4G" << "rj45" << "tf" << "eeprom" << "wifi";
    memset(m_errCount,0,sizeof(m_errCount));
    this->createTable();
    this->createUi();
    this->setFixedSize(350,410);
}

DeviceWindow::~DeviceWindow()
{

}

void DeviceWindow::createTable()
{
    m_table = new QTableWidget;
    QStringList header,v_header;
    header<<"项"<<"err次数"<<"当前状态";
    m_table->setColumnCount(CJ_COLUMN);
    m_table->setRowCount(CJ_ROW);
    m_table->setHorizontalHeaderLabels(header);
    m_table->verticalHeader()->setHidden(true);
    m_table->horizontalHeader()->setStyleSheet("QHeaderView{border:0;border-bottom:1px solid #000;}");
    m_table->horizontalHeader()->setStretchLastSection(true);
    m_table->verticalHeader()->setStretchLastSection(true);
    m_table->setEditTriggers(QAbstractItemView::NoEditTriggers);
    m_table->setSelectionBehavior(QAbstractItemView::SelectItems);
    for(int i=0;i<m_items.count();i++){
        QTableWidgetItem *item = new QTableWidgetItem(m_items[i]);
        QTableWidgetItem *err  = new QTableWidgetItem("0");
        QTableWidgetItem *status = new QTableWidgetItem("--");
        item->setTextAlignment(Qt::AlignCenter);
        err->setTextAlignment(Qt::AlignCenter);
        status->setTextAlignment(Qt::AlignCenter);
        status->setFont( QFont( "Times", 12, QFont::Black));
        m_table->setItem(i,0,item);
        m_table->setItem(i,1,err);
        m_table->setItem(i,2,status);
    }
}

void DeviceWindow::createUi()
{
    QVBoxLayout *vbox  = new QVBoxLayout;
    QHBoxLayout *hbox1 = new QHBoxLayout;
    QHBoxLayout *hbox2 = new QHBoxLayout;
    QHBoxLayout *hbox3 = new QHBoxLayout;
    QHBoxLayout *hbox4 = new QHBoxLayout;
    QLabel *label_ip   = new QLabel("ip  : ");
    QLabel *label_mac  = new QLabel("Mac : ");
    QLabel *label_tmp1 = new QLabel("板子温度 : ");
    QLabel *label_tmp2 = new QLabel("CPU温度 : ");
    QLabel *label_time = new QLabel("设备时间 : ");
    m_ipLabel = new QLabel;
    m_macLabel = new QLabel;
    m_tmp1Label = new QLabel;
    m_tmp2Label = new QLabel;
    m_timeLabel = new QLabel;
    QPushButton *tf = new QPushButton("测tf");
    QPushButton *eeprom = new QPushButton("测eeprom");
    QPushButton *rtc_get = new QPushButton("获取Rtc");
    QPushButton *rtc_update = new QPushButton("更新Rtc");
    hbox1->addWidget(label_ip); hbox1->addWidget(m_ipLabel);
    hbox2->addWidget(label_mac);hbox2->addWidget(m_macLabel);
    hbox3->addWidget(label_tmp1);hbox3->addWidget(m_tmp1Label);
    hbox3->addWidget(label_tmp2);hbox3->addWidget(m_tmp2Label);
    hbox3->addWidget(label_time);hbox3->addWidget(m_timeLabel);
    hbox4->addWidget(tf);hbox4->addWidget(eeprom);
    hbox4->addWidget(rtc_get);hbox4->addWidget(rtc_update);
    vbox->addLayout(hbox1);vbox->addLayout(hbox2);vbox->addLayout(hbox3);
    vbox->addWidget(m_table);
    vbox->addLayout(hbox4);
    this->setLayout(vbox);
}
