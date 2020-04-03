#include <QBoxLayout>
#include <QPushButton>
#include <QLabel>
#include <QLineEdit>
#include "setting.h"
#include "mydialog.h"


MyDialog::MyDialog()
{
    this->setWindowTitle("添加ip");
    this->setMinimumSize(200,200);
    QVBoxLayout *v = new QVBoxLayout;
    QHBoxLayout *h1 =new QHBoxLayout;
    QHBoxLayout *h2 =new QHBoxLayout;
    QHBoxLayout *h3 =new QHBoxLayout;

    QString ip = Setting::ins()->getEditIp();
    QString port = Setting::ins()->getEditPort();
    if(ip.isEmpty())ip = "192.168.1.1";
    if(port.isEmpty())port = "12301";

    QLabel *label_ip   = new QLabel("ip   : ");
    QLabel *label_port = new QLabel("port : ");
    m_ipEdit = new QLineEdit(ip);
    m_portEdit = new QLineEdit(port);
    m_portEdit->setReadOnly(true);
    QPushButton *bt_yes = new QPushButton("确定");
    QPushButton *bt_no  = new QPushButton("取消");

    h1->addWidget(label_ip);h1->addWidget(m_ipEdit);
    h2->addWidget(label_port);h2->addWidget(m_portEdit);
    h3->addWidget(bt_yes);h3->addWidget(bt_no);
    v->addLayout(h1);v->addLayout(h2);v->addLayout(h3);
    this->setLayout(v);

    connect(bt_yes,&QPushButton::clicked,this,&MyDialog::yesClicked);
    connect(bt_no,&QPushButton::clicked,this,&MyDialog::noClicked);
    connect(m_ipEdit,&QLineEdit::editingFinished,this,&MyDialog::ipFinished);
    connect(m_portEdit,&QLineEdit::editingFinished,this,&MyDialog::portFinished);
}

MyDialog::~MyDialog()
{

}

void MyDialog::yesClicked()
{
    QString ip = m_ipEdit->text();
    int port = m_portEdit->text().toUInt();
    emit addIp(ip,port);
    this->hide();
}

void MyDialog::noClicked()
{
    this->hide();
}

void MyDialog::ipFinished()
{
    Setting::ins()->setEditIp(m_ipEdit->text());
}

void MyDialog::portFinished()
{
    Setting::ins()->setEditPort(m_portEdit->text());
}
