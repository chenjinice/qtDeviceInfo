#include <QBoxLayout>
#include <QPushButton>
#include <QIntValidator>
#include <QRegExpValidator>
#include <QFont>
#include <QDebug>
#include <QLabel>
#include <QLineEdit>
#include "setting.h"
#include "mydialog.h"


MyDialog::MyDialog()
{
    int   width             = 50;
    QString dot             = ".";
    QFont font( "Microsoft YaHei",10,75);
    QRegExp           rx("^(25[0-5]|2[0-4][0-9]|[0-1]?[0-9]?[0-9])");
    QRegExpValidator *reg = new QRegExpValidator(rx, this);

    this->setWindowTitle(tr("+"));
    this->setMinimumSize(300,200);
    QVBoxLayout *v          = new QVBoxLayout;
    QHBoxLayout *h1         =new QHBoxLayout;
    QHBoxLayout *h_from     =new QHBoxLayout;
    QHBoxLayout *h_to       =new QHBoxLayout;
    QHBoxLayout *h2         =new QHBoxLayout;

    QString ip = Setting::ins()->getEditIp();
    if(ip.isEmpty())ip      = "192.168.1.1";

    QLabel *label_ip        = new QLabel("ip   : ");
    QLabel *   d1           = new QLabel(".");d1->setFont(font);
    QLabel *   d2           = new QLabel(".");d2->setFont(font);
    QLabel *   d3           = new QLabel(".");d3->setFont(font);
    QLabel *   d5           = new QLabel(".");d5->setFont(font);
    QLabel *   d6           = new QLabel(".");d6->setFont(font);
    QLabel *   d7           = new QLabel(".");d7->setFont(font);
    m_ipEdit                = new QLineEdit(ip);
    m_ip1                   = new QLineEdit();m_ip1->setFixedWidth(width);m_ip1->setValidator(reg);
    m_ip2                   = new QLineEdit();m_ip2->setFixedWidth(width);m_ip2->setValidator(reg);
    m_ip3                   = new QLineEdit();m_ip3->setFixedWidth(width);m_ip3->setValidator(reg);
    m_ip4                   = new QLineEdit();m_ip4->setFixedWidth(width);m_ip4->setValidator(reg);
    m_to1                   = new QLabel();   m_to1->setFixedWidth(width);
    m_to2                   = new QLabel();   m_to2->setFixedWidth(width);
    m_to3                   = new QLabel();   m_to3->setFixedWidth(width);
    m_to4                   = new QLineEdit();m_to4->setFixedWidth(width);m_to4->setValidator(reg);

    QPushButton *bt_yes = new QPushButton("确定");
    QPushButton *bt_no  = new QPushButton("取消");



    h1->addWidget(label_ip);h1->addWidget(m_ipEdit);
    h_from->addWidget(m_ip1);h_from->addWidget(d1);h_from->addWidget(m_ip2);h_from->addWidget(d2);
    h_from->addWidget(m_ip3);h_from->addWidget(d3);h_from->addWidget(m_ip4);
    h_to->addWidget(m_to1);h_to->addWidget(d5);h_to->addWidget(m_to2);h_to->addWidget(d6);
    h_to->addWidget(m_to3);h_to->addWidget(d7);h_to->addWidget(m_to4);
    h2->addWidget(bt_yes);h2->addWidget(bt_no);
    v->addLayout(h1);v->addLayout(h_from);
    v->addLayout(h_to);v->addLayout(h2);
    this->setLayout(v);

    connect(bt_yes,&QPushButton::clicked,this,&MyDialog::yesClicked);
    connect(bt_no,&QPushButton::clicked,this,&MyDialog::noClicked);
    connect(m_ipEdit,&QLineEdit::editingFinished,this,&MyDialog::ipFinished);
}

MyDialog::~MyDialog()
{
}

void MyDialog::yesClicked()
{
//    for(int i=0;i<10;i++){
//        QString str = "127.0.0.1";
//        emit addIp(str);
//    }
    QString ip = m_ipEdit->text();
    emit addIp(ip);
    this->hide();
}

void MyDialog::noClicked()
{
    this->hide();
}

void MyDialog::ipFinished()
{
    qDebug() << "fffffff";
    Setting::ins()->setEditIp(m_ipEdit->text());
}

