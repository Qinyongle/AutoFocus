#include "datareciever.h"

DataReciever::DataReciever(QObject *parent) : QObject(parent)
{

}
void DataReciever::setHead(QByteArray h)
{
    head = h;
}

void DataReciever::setTail(QByteArray t)
{
    tail = t;
}
void DataReciever::DoRecive(QByteArray)
{

}
