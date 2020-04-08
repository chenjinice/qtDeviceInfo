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
#include "common.h"
#include "udpthread.h"


MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
{
    m_udpThread = UdpThread::ins();
    m_table = new MyTable;
    connect(m_udpThread,&UdpThread::bindState,this,&MainWindow::udpState);
    connect(m_udpThread,&UdpThread::getIp,m_table,&MyTable::addIp);
    this->createCentralWidget();
    this->createCombo();
    this->createButton();
    m_layout->addWidget(m_table);
    this->setMinimumSize(1000,500);
    this->setWindowTitle("测试软件 - V2020.04.08");
}

MainWindow::~MainWindow()
{
    delete m_udpThread;
    delete m_table;
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
    QStringList l = getSelfIp();
    QLabel *label_card     = new QLabel("Udp ：");
    m_labelState           = new QLabel;
    m_labelState->setFixedWidth(15);
    QComboBox *combo = new QComboBox;
    for(int i=0;i<l.count();i++){
        combo->addItem(l.at(i));
        if(l.at(i) == Setting::ins()->getCard()){
            combo->setCurrentText(l.at(i));
        }
    }
    hbox->addWidget(label_card);
    hbox->addWidget(combo);
    hbox->addWidget(m_labelState);
    if(!combo->currentText().isEmpty()){
        QString ip = combo->currentText();
        m_udpThread->startThread(ip);
    }
    m_layout->addLayout(hbox);
    connect(combo,&QComboBox::currentTextChanged,m_udpThread,&UdpThread::setIp);
}

void MainWindow::createButton()
{
    QHBoxLayout *hbox = new QHBoxLayout;
    hbox->setAlignment(Qt::AlignLeft);
    QLabel *label_other = new QLabel("其他 ：");
    QCheckBox *check_box = new QCheckBox("自动连接");
    QPushButton *sort_bt = new QPushButton("按IP排序");
    QPushButton *clear = new QPushButton("清表格");
    QPushButton *add = new QPushButton("添IP");
    hbox->addWidget(label_other);
    hbox->addWidget(check_box);
    hbox->addWidget(sort_bt);
    hbox->addWidget(clear);
    hbox->addWidget(add);
    m_layout->addLayout(hbox);
    check_box->setChecked(Setting::ins()->getMode());
    connect(add,&QPushButton::clicked,this,&MainWindow::addClicked);
    connect(clear,&QPushButton::clicked,this,&MainWindow::clearClicked);
    connect(check_box,&QCheckBox::stateChanged,this,&MainWindow::checkBoxChanged);
    connect(sort_bt,&QPushButton::clicked,m_table,&MyTable::sortByIp);
}

void MainWindow::udpState(bool flag)
{
    QString color = "background-color: rgb(255,0,0);border-radius:6px;";
    if(flag)color = "background-color: rgb(0,255,0);border-radius:6px;";
    m_labelState->setStyleSheet(color);
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
        connect(s_dialog,&MyDialog::addIp,m_table,&MyTable::addIp);
    }
    s_dialog->show();
}

void MainWindow::checkBoxChanged(int value)
{
    bool flag = false;
    if(value > 0)flag = true;
    Setting::ins()->setMode(flag);
}

