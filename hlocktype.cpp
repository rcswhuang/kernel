#include "hlocktype.h"

HLockType::HLockType()
{

}

HLockType::~HLockType()
{

}

ushort HLockType::stationID()
{
    return wfLockType.wStationID;
}

ushort HLockType::lockTypeID()
{
    return wfLockType.wLockTypeID;
}

uchar  HLockType::lockType()
{
    return wfLockType.szLockType[0];
}

QString HLockType::lockTypeName()
{
    return (QString)wfLockType.szLockTypeName;
}
