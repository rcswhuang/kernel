﻿#ifndef HKERSTATION_H
#define HKERSTATION_H

#include <QObject>
#include <QList>
#include "hkernelapi.h"
#include "hkerdigital.h"
class HKerWord;
//class HKerDigital;
class HKerAnalogue;
class HKerGroup;

class HKerStation : public QObject
{
    Q_OBJECT
public:
    HKerStation(QObject* parent = 0);
   ~HKerStation();

public:
    bool loadData(FILEHANDLE &fileHandle);
    void saveData(FILEHANDLE &fileHandle);
    quint16 getNo();
    quint16 getRtuNo();
    const char* getName();
    bool getLockStatus();

    HKerWord* kerWord(quint8 btType,quint16 wNo);
    HKerAnalogue* kerAnalogue(quint16 wNo);
    HKerDigital* kerDigital(quint16 wNo);
    HKerGroup* kerGroup(quint16 wNo);

    //获取间隔投入的遥信
    HKerDigital* groupStateDigital(quint16 wGroupID);

    HKerWord* findKerWord(quint8 btType,quint16 wIndex);
    HKerAnalogue* findKerAnalogue(quint16 wIndex);
    HKerDigital* findKerDigital(quint16 wIndex);
    HKerGroup* findKerGroup(quint16 wIndex);
    TPDIGITAL* findTPDigital(HKerDigital* pKerD);

    bool getAttr(quint16 wAttrib,void* pValue,quint32 size = 0);
    bool setAttr(quint16 wAttrib,void* pValue);
    bool doSetAttr(quint16 wAttrib,void* pValue);
public:
    quint16 m_wTotalAnalogue;
    quint16 m_wTotalDigital;
    quint16 m_wTotalGroup;

    HKerAnalogue* m_pKerAnalogue;//遥测存放的内存起始地址
    HKerDigital* m_pKerDigital;
    HKerGroup* m_pKerGroup;

    QList<TPDIGITAL*> m_pTpDigitalList;

public:
    STATION station;
};

#endif // HKERSTATION_H
