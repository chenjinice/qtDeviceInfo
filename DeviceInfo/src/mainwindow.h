#ifndef MAINWINDOW_H
#define MAINWINDOW_H


class AllIp;
class MyTable;
class QVBoxLayout;
class QCheckBox;

#include <QMainWindow>
#include "allip.h"


class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();

private:
    QVBoxLayout  * m_layout;
    QCheckBox    * m_checkBox;
    AllIp        * m_allIp;
    MyTable      * m_table;

    void createCentralWidget();
    void createCombo();
    void createButton();
    void createTable();

public slots:
    void textChanged(QString str);
    void clearClicked();
    void addClicked();
    void getAllIp(QList<IpData> l);
};

#endif // MAINWINDOW_H
