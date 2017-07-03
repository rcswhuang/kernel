#ifndef HKERANALOGUE_H
#define HKERANALOGUE_H
#include "kerword.h"
#include "publicdata.h"

class HKerAnalogue : public HKerWord
{
public:
    HKerAnalogue();
    ~HKerAnalogue();

public:
    void doWrite(float fValue,HKerStation* pStation,long lTime);
    void checkValue(float fValue,HKerStation* pStation,long lTime);
    bool writeValue(float fValue,HKerStation* pStation,long lTime);
    //结果标志信息
    bool isResult(quint16 wResult);
    void setResult(quint16 wResult);
    void deSetResult(quint16 wResult);

    //人工置数等信息
    void doManualSet(float fValue,HKerStation* pStation);
    void manualSet(float fValue,HKerStation* pStation);
    void doDeManualSet(HKerStation* pStation);
    void deManualSet(HKerStation* pStation);

    //自身属性获取
    quint16 getNo();
    quint8 getType();
    quint16 getIndex(HKerStation* pStation);
    const char* getName();

    //设置实时值和获取实时值
    bool getAttr(quint16 wAttrib, void *pValue, HKerStation *pStation = NULL, quint32 size = 0);
    bool setAttr(quint16 wAttrib, void *pValue, HKerStation *pStation);
    bool doSetAttr(quint16 wAttrib, void *pValue, HKerStation *pStation);

public:
    ANALOGUE analogue;
};

#endif // HKERANALOGUE_H
