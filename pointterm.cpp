#include "pointterm.h"

HPointTerm::HPointTerm(QObject *parent) : QObject(parent)
{

}

HPointTerm::~HPointTerm()
{

}

quint16 HPointTerm::pointTermID()
{
    return pointTerm.wTermID;
}

QString HPointTerm::pointTermName()
{
    return (QString)pointTerm.szTermName;
}

uchar HPointTerm::pointTermType()
{
    return pointTerm.btType;
}
