#include "hkergroup.h"
#include "hkerstation.h"
HKerGroup::HKerGroup()
{

}

HKerGroup::~HKerGroup()
{

}

quint16 HKerGroup::getNo()
{
    return equipmentGroup.wGroupID;
}

quint8 HKerGroup::getType()
{
    return TYPE_GROUPIP;
}

quint16 HKerGroup::getIndex(HKerStation* pStation)
{
    if(pStation)
    {
        return (this - pStation->m_pKerGroup);
    }
    return (quint16)-1;
}

QString HKerGroup::getName()
{
    return (QString)equipmentGroup.szGroupName;
}

//设值
bool HKerGroup::getAttr(quint16 wAttrib, void *pValue, HKerStation *pStation, quint32 size)
{
    return false;
}

bool HKerGroup::setAttr(quint16 wAttrib, void *pValue, HKerStation *pStation)
{
    return false;
}

bool HKerGroup::doSetAttr(quint16 wAttrib, void *pValue, HKerStation *pStation)
{
    return false;
}
