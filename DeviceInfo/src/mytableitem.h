#ifndef MYTABLEITEM_H
#define MYTABLEITEM_H

#define C_IDROLE   0x100
#define C_PORTROLE 0x101
#define C_ERRROLE  0x102

#include <QTableWidgetItem>


class MyTableItem : public QTableWidgetItem
{
public:
    MyTableItem();
    ~MyTableItem();
    bool operator < (const QTableWidgetItem &other) const;

private:

};

#endif // MYTABLEITEM_H
