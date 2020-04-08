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

    QString ip = Setting::ins()->getEditIp();
    if(ip.isEmpty())ip = "192.168.1.1";

    QLabel *label_ip   = new QLabel("ip   : ");
    m_ipEdit = new QLineEdit(ip);
    QPushButton *bt_yes = new QPushButton("确定");
    QPushButton *bt_no  = new QPushButton("取消");

    h1->addWidget(label_ip);h1->addWidget(m_ipEdit);
    h2->addWidget(bt_yes);h2->addWidget(bt_no);
    v->addLayout(h1);v->addLayout(h2);
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
    Setting::ins()->setEditIp(m_ipEdit->text());
}

