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
    this->setMinimumSize(500,500);
    this->setWindowTitle("测试软件 - V2020.07.02");
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
    QLabel *label_card     = new QLabel(tr("Udp ："));
    m_labelState           = new QLabel;
    m_labelState->setFixedSize(15,15);
    m_labelState->setScaledContents(true);
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
    QLabel *label_other = new QLabel(tr("其他 ："));
    QCheckBox *sort_box = new QCheckBox(tr("按ip排序"));
    QPushButton *clear = new QPushButton(tr("清表格"));
    QPushButton *add = new QPushButton(tr("添ip"));
    hbox->addWidget(label_other);
//    hbox->addWidget(check_box);
    hbox->addWidget(sort_box);
    hbox->addWidget(clear);
    hbox->addWidget(add);
    m_layout->addLayout(hbox);
    sort_box->setChecked(Setting::ins()->getAutoSort());
    connect(add,&QPushButton::clicked,this,&MainWindow::addClicked);
    connect(clear,&QPushButton::clicked,this,&MainWindow::clearClicked);
    connect(sort_box,&QCheckBox::stateChanged,this,&MainWindow::sortBoxChanged);
}

void MainWindow::udpState(bool flag)
{
    static QPixmap yes(":/icon/image/connect_yes.png");
    static QPixmap no(":/icon/image/connect_no.png");
    if(flag)m_labelState->setPixmap(yes);
    else m_labelState->setPixmap(no);
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
    s_dialog->setWindowFlags(Qt::WindowStaysOnTopHint);
    s_dialog->show();
}

void MainWindow::sortBoxChanged(int value)
{
    bool flag = false;
    if(value > 0){
        flag = true;
        m_table->sortByIp();
    }
    Setting::ins()->setAutoSort(flag);
}

