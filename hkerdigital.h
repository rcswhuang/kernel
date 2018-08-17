#ifndef HKERDIGITAL_H
#define HKERDIGITAL_H
#include "hkerword.h"
#include "publicdata.h"
//双位置遥信
class HKerStation;
class HKerDigital;
//双位置点信息
#define TP_NORMAL 0
#define TP_WAITA  1
#define TP_WAITB  2
typedef struct//
{
    ushort wWaitCount;//双位置的判断时间
    HKerDigital* pDigitalA,*pDigitalB;
    uchar btOldValue;
    uchar btState;
}TPDIGITAL;//two-position Digital 双位置遥信点


class HKerDigital : public HKerWord
{
public:
    HKerDigital();
    ~HKerDigital();

public:
    //标志结果等信息
    bool isEnable(int wEnable);//获取一些允许的属性 比如允许取反 允许报警等
    bool isResult(int wResult);//结果的属性 比如人工置数、确认、正常变位等
    void enableFlag(int wFlag, bool bEnable, HKerStation *pStation);
    void doEnableFlag(int wFlag, bool bEnable, HKerStation *pStation);
    void setResult(int wResult);//使具有一些结果属性
    void deSetResult(int wResult);//取消结果属性

    bool isOpen(quint8 btValue);
    bool isClose(quint8 btValue);

    //双位置等信息
    void checkDS(HKerStation* pStation,long lTime);
    void checkValue(quint8 btValue,HKerStation* pStation,long lTime);
    void doWrite(quint8 btValue,HKerStation* pStation,long lTime);
    bool writeValue(quint8 btValue,HKerStation* pStation,long lTime);

    //确认 人工置数等
    void ack(HKerStation* pStation,long lTime);
    void doManualSet(quint8 btValue,HKerStation* pStation);
    void manualSet(quint8 btValue,HKerStation* pStation);
    void doDeManualSet(HKerStation* pStation);
    void deManualSet(HKerStation* pStation);

    //属性信息
    quint16 getIndex(HKerStation* pStation);
    quint16 getNo();
    quint8  getType();
    const char* getName();
    quint16 groupID();

    bool getAttr(quint16 wAttrib, void *pValue, HKerStation *pStation, quint32 size = 0);
    bool setAttr(quint16 wAttrib, void *pValue, HKerStation *pStation);
    bool doSetAttr(quint16 wAttrib, void *pValue, HKerStation *pStation);


public:
    DIGITAL digital;
    TPDIGITAL* m_pTPDigital;
};

#endif // HKERDIGITAL_H
