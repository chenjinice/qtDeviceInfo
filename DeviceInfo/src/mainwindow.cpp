#include <QPushButton>
#include <QBoxLayout>
#include <QComboBox>
#include <QCheckBox>
#include <QSettings>
#include <QLabel>
#include <QDebug>
#include "mainwindow.h"
#include "setting.h"
#include "mytable.h"
#include "mydialog.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_allIp = new AllIp;
    this->setWindowTitle("测试软件 - V2020.04.03");
    this->createCentralWidget();
    this->createCombo();
    this->createButton();
    this->createTable();
    this->setMinimumSize(1000,500);
    connect(m_allIp,&AllIp::sent,this,&MainWindow::getAllIp);
}

MainWindow::~MainWindow()
{
    delete  m_allIp;
    delete  m_table;
}

void MainWindow::createCentralWidget()
{
    QWidget *widget = new QWidget;
    m_layout = new QVBoxLayout;
    widget->setLayout(m_layout);
    this->setCentralWidget(widget);
}

void MainWindow::createCombo()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setAlignment(Qt::AlignLeft);
    QStringList l = AllIp::getSelfIp();
    QLabel *label_card     = new QLabel("网卡 ：");
    QComboBox *combo = new QComboBox;
    for(int i=0;i<l.count();i++){
        combo->addItem(l.at(i));
        if(l.at(i) == Setting::ins()->getCard()){
            combo->setCurrentText(l.at(i));
        }
    }
    hbox->addWidget(label_card);
    hbox->addWidget(combo);
    if(!combo->currentText().isEmpty()){
        QString ip = combo->currentText();
        m_allIp->setCard(ip);
    }
    m_layout->addLayout(hbox);
    connect(combo,&QComboBox::currentTextChanged,this,&MainWindow::textChanged);
}

void MainWindow::createButton()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setAlignment(Qt::AlignLeft);
    QLabel *label_other = new QLabel("其他 ：");
    m_checkBox = new QCheckBox("自动连接");
    QPushButton *add = new QPushButton("添加ip");
    QPushButton *clear = new QPushButton("清空表格");
    hbox->addWidget(label_other);
//    hbox->addWidget(m_checkBox);
    hbox->addWidget(add);
    hbox->addWidget(clear);
    m_layout->addLayout(hbox);
    connect(add,&QPushButton::clicked,this,&MainWindow::addClicked);
    connect(clear,&QPushButton::clicked,this,&MainWindow::clearClicked);
}

void MainWindow::createTable()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    m_table = new MyTable;
    hbox->addWidget(m_table);
    m_layout->addLayout(hbox);
}

void MainWindow::textChanged(QString str)
{
    Setting *s =  Setting::ins();
    s->setCard(str);
    m_allIp->setCard(str);
}

void MainWindow::clearClicked()
{
    m_table->clearResult();
}

void MainWindow::addClicked()
{
    static MyDialog *s_dialog = nullptr;
    if(s_dialog == nullptr){
        s_dialog = new MyDialog;
        connect(s_dialog,&MyDialog::addIp,m_table,&MyTable::addSingleIp);
    }
    s_dialog->show();
}

void MainWindow::getAllIp(QList<IpData> l)
{
    m_table->addClient(l);

//    static int i = 0;
//    if(i++ > 0)return;
//    QList<IpData> list;
//    for(int i=0;i<4;i++){
//        IpData d;
//        d.ip = "127.0.0.1";
//        d.port = C_PORT+i;
//        d.id = AllIp::ipToId(d.ip);
//        d.cmd = IP_ADD;
//        list.append(d);
//    }
//    m_table->addClient(list);
}
