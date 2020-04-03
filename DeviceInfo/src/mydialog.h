#ifndef MYDIALOG_H
#define MYDIALOG_H

class QLineEdit;

#include <QDialog>


class MyDialog : public QDialog
{
    Q_OBJECT
public:
    MyDialog();
    ~MyDialog();

private:
    QLineEdit *m_ipEdit;
    QLineEdit *m_portEdit;

    void yesClicked();
    void noClicked();
    void ipFinished();
    void portFinished();

signals:
    void addIp(QString ip,int port);
};


#endif // MYDIALOG_H
