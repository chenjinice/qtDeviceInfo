#ifndef DEVICEWINDOW_H
#define DEVICEWINDOW_H

class QLabel;
class QTextEdit;
class QPushButton;
class QTableWidget;

#include <QWidget>

#define CJ_COLUMN 3
#define CJ_ROW    9


class DeviceWindow : public QWidget
{
    Q_OBJECT
public:
    DeviceWindow(QWidget *parent = nullptr);
    ~DeviceWindow();

private:
    QLabel        * m_ipLabel;
    QLabel        * m_macLabel;
    QLabel        * m_tmp1Label;
    QLabel        * m_tmp2Label;
    QLabel        * m_timeLabel;
    int             m_errCount[CJ_ROW];
    QStringList     m_items;
    QTableWidget  * m_table;

    void createTable();
    void createUi();


signals:

public slots:
};

#endif // DEVICEWINDOW_H
