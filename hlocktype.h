#ifndef HLOCKTYPE_H
#define HLOCKTYPE_H
#include "publicdata.h"
#include <QObject>

class HLockType : public QObject
{
public:
    HLockType();
   ~HLockType();

public:
    ushort stationID();
    ushort lockTypeID();
    uchar  lockType();
    QString lockTypeName();
public:
    WFLOCKTYPE wfLockType;


};

#endif // HLOCKTYPE_H
