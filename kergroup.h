#ifndef HKERGROUP_H
#define HKERGROUP_H
#include "kerword.h"
#include "publicdata.h"
class HKerStation;
class HKerGroup : public HKerWord
{
public:
    HKerGroup();
    ~HKerGroup();

public:
    quint16 getNo();
    quint8 getType();
    quint16 getIndex(HKerStation* pStation);
    QString getName();

    //设值
    bool getAttr(quint16 wAttrib, void *pValue, HKerStation *pStation, quint32 size = 0);
    bool setAttr(quint16 wAttrib, void *pValue, HKerStation *pStation);
    bool doSetAttr(quint16 wAttrib, void *pValue, HKerStation *pStation);

public:
    EQUIPMENTGROUP equipmentGroup;
};

#endif // HKERGROUP_H
