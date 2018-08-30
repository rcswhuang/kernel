#include "hpointterm.h"

HPointTerm::HPointTerm(QObject *parent) : QObject(parent)
{

}

HPointTerm::~HPointTerm()
{

}

POINTTERM* HPointTerm::getPointTerm()
{
    return &pointTerm;
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
