#ifndef MYDIALOG_H
#define MYDIALOG_H

class QLineEdit;

#include <QDialog>
#include "common.h"

class MyDialog : public QDialog
{
    Q_OBJECT
public:
    MyDialog();
    ~MyDialog();

private:
    QLineEdit *m_ipEdit;

    void yesClicked();
    void noClicked();
    void ipFinished();

signals:
    void addIp(QString ip);
};


#endif // MYDIALOG_H
