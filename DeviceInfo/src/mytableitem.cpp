#include "mytableitem.h"


MyTableItem::MyTableItem()
{
    this->setTextAlignment(Qt::AlignCenter);
    this->setData(C_ERRROLE,0);
}

MyTableItem::~MyTableItem()
{

}

bool MyTableItem::operator <(const QTableWidgetItem &other) const
{
    int id = this->data(C_IDROLE).toUInt();
    int that_id = other.data(C_IDROLE).toUInt();
    return id < that_id;
}
