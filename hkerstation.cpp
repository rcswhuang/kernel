#include "hkerstation.h"
#include "hkeranalogue.h"
//#include "kerdigital.h"
#include "hkergroup.h"
#include "hlocktype.h"

HKerStation::HKerStation(QObject* parent):QObject(parent)
{
    wTotalAnalogue = 0;
    wTotalDigital = 0;
    wTotalGroup = 0;

    pKerAnalogue = NULL;//遥测存放的内存起始地址
    pKerDigital = NULL;
    pKerGroup = NULL;
}

HKerStation::~HKerStation()
{
    wTotalAnalogue = 0;
    wTotalDigital = 0;
    wTotalGroup = 0;


    if(pKerAnalogue)
    {
        delete[] pKerAnalogue;
        pKerAnalogue = NULL;
    }
    if(pKerDigital)
    {
        delete[] pKerDigital;
        pKerDigital = NULL;
    }

    if(pKerGroup)
    {
        delete[] pKerGroup;
        pKerGroup = NULL;
    }

}

bool HKerStation::loadData(FILEHANDLE &fileHandle)
{
    wTotalAnalogue = station.wAnalogueCounts;
    wTotalDigital = station.wDigitalCounts;
    wTotalGroup = station.wEquipmentGroupCounts;
    //加载文件
    //间隔
  /*  openDB(FILE_TYPE_EQUIPMENTGROUP);
    pKerGroup = new HKerGroup[wTotalGroup];
    HKerGroup* pKerG = pKerGroup;
    for(int i = 0; i < wTotalGroup;i++)
    {
        if(false == loadDBRecord(FILE_TYPE_EQUIPMENTGROUP,++fileHandle.wEquipmentGroup,&pKerG->equipmentGroup))
        {
            delete pKerGroup;
            pKerGroup = NULL;
            return false;
        }
    }*/

    //遥测
    openDB(FILE_TYPE_ANALOGUE);
    pKerAnalogue = new HKerAnalogue[wTotalAnalogue];
    HKerAnalogue* pKerA = pKerAnalogue;
    for(int i = 0; i < wTotalAnalogue;i++,pKerA++)
    {
        if(false == loadDBRecord(FILE_TYPE_ANALOGUE,++fileHandle.wAnalogue,&pKerA->analogue))
        {
            delete pKerAnalogue;
            return false;
        }
    }

    //遥信
    openDB(FILE_TYPE_DIGITAL);
    pKerDigital = new HKerDigital[wTotalDigital];
    HKerDigital* pKerD = pKerDigital;
    for(int i = 0; i < wTotalDigital;i++,pKerD++)
    {
        if(false == loadDBRecord(FILE_TYPE_DIGITAL,++fileHandle.wDigital,&pKerD->digital))
        {
            delete[] pKerDigital;
            pKerDigital = NULL;
            return false;
        }
    }

    //加载双位置遥信
    pKerD = pKerDigital;
    HKerDigital* pKerD1 = NULL;
    for(int i = 0; i < wTotalDigital;i++,pKerD++)
    {
        if(pKerD->digital.wDoubleDgtID == (ushort)-1)
            continue;
        pKerD1 = kerDigital(pKerD->digital.wDoubleDgtID);
        if(!pKerD1) continue;
        bool bfind = false;
        for(int j = 0; j < m_pTpDigitalList.count();j++)
        {
            TPDIGITAL* tpDigital = (TPDIGITAL*)m_pTpDigitalList[j];
            if(tpDigital->pDigitalA->getNo() == pKerD->getNo() || tpDigital->pDigitalB->getNo() == pKerD->getNo())
            {
                bfind = true;
                break;
            }
        }
        if(!bfind)
        {
            TPDIGITAL* tpDigital = new TPDIGITAL;
            tpDigital->btState = TP_NORMAL;
            tpDigital->pDigitalA = pKerD;
            tpDigital->pDigitalB = pKerD1;
            tpDigital->pDigitalA->m_pTPDigital = tpDigital;
            tpDigital->pDigitalB->m_pTPDigital = tpDigital;

            m_pTpDigitalList.append(tpDigital);
        }
    }

    closeDB(FILE_TYPE_EQUIPMENTGROUP);
    closeDB(FILE_TYPE_ANALOGUE);
    closeDB(FILE_TYPE_DIGITAL);
    return true;
}

void HKerStation::saveData(FILEHANDLE &fileHandle)
{
    //间隔
    createDB(FILE_TYPE_EQUIPMENTGROUP);
    HKerGroup* pKerG = pKerGroup;
    for(int i = 0; i < wTotalGroup;i++)
    {
        if(false == saveDBRecord(FILE_TYPE_EQUIPMENTGROUP,++fileHandle.wEquipmentGroup,&pKerG->equipmentGroup))
        {
            break;
        }
    }

    //遥测
    createDB(FILE_TYPE_ANALOGUE);
    HKerAnalogue* pKerA = pKerAnalogue;
    for(int i = 0; i < wTotalAnalogue;i++,pKerA++)
    {
        if(false == saveDBRecord(FILE_TYPE_ANALOGUE,++fileHandle.wAnalogue,&pKerA->analogue)){   break;  }
    }

    //遥信
    createDB(FILE_TYPE_DIGITAL);
    HKerDigital* pKerD = pKerDigital;
    for(int i = 0; i < wTotalDigital;i++,pKerD++)
    {
        if(false == saveDBRecord(FILE_TYPE_DIGITAL,++fileHandle.wDigital,&pKerD->digital))
        {
            break;
        }
    }

    closeDB(FILE_TYPE_EQUIPMENTGROUP);
    closeDB(FILE_TYPE_ANALOGUE);
    closeDB(FILE_TYPE_DIGITAL);
}

quint16 HKerStation::getNo()
{
    return station.wStationID;
}

quint16 HKerStation::getRtuNo()
{
    return station.wStationAddress;
}

const char* HKerStation::getName()
{
    return station.szStationName;
}

bool HKerStation::getLockStatus()
{
    return station.btStationLock;
}

HKerWord* HKerStation::kerWord(quint8 btType,quint16 wNo)
{
    switch(btType)
    {
    case TYPE_ANALOGUE: return kerAnalogue(wNo);break;
    case TYPE_DIGITAL:  return kerDigital(wNo);break;
    case TYPE_GROUP:    return kerGroup(wNo);break;
    default:break;
    }
    return NULL;
}

HKerAnalogue* HKerStation::kerAnalogue(quint16 wNo)
{
    if(NULL == pKerAnalogue)
        return NULL;
    HKerAnalogue* pKerAna = (HKerAnalogue*)pKerAnalogue;
    for(int i = 0; i < wTotalAnalogue;i++,pKerAna++)
    {
        if(pKerAna->getNo() == wNo)
            return pKerAna;
    }
    return NULL;
}

HKerDigital* HKerStation::kerDigital(quint16 wNo)
{
    if(NULL == pKerDigital)
        return NULL;
    HKerDigital* pKerDig = (HKerDigital*)pKerDigital;
    for(int i = 0; i < wTotalDigital;i++,pKerDig++)
    {
        if(pKerDig->getNo() == wNo)
            return pKerDig;
    }
    return NULL;
}

HKerGroup* HKerStation::kerGroup(quint16 wNo)
{
    if(NULL == pKerGroup)
        return NULL;
    HKerGroup* pKerG = (HKerGroup*)pKerGroup;
    for(int i =0; i < wTotalGroup;i++,pKerG++)
    {
        if(pKerG->getNo() == wNo)
            return pKerG;
    }
    return NULL;
}

/*
HLockType* HKerStation::lockType(quint16 wNo)
{
    if(NULL == pLockType)
        return NULL;
    HLockType* pLT = pLockType;
    for(int i = 0; i < wTotalLock;i++,pLT++)
    {
        if(pLT->lockTypeID() == wNo)
            return pLT;
    }
}*/


HKerWord* HKerStation::findKerWord(quint8 btType,quint16 wIndex)
{
    switch(btType)
    {

    case TYPE_DIGITAL:  return findKerDigital(wIndex);break;
    case TYPE_GROUP:    return findKerGroup(wIndex);break;
    case TYPE_ANALOGUE: return findKerAnalogue(wIndex);break;
    default:break;
    }
    return NULL;
}

HKerAnalogue* HKerStation::findKerAnalogue(quint16 wIndex)
{
    if(NULL == pKerAnalogue || wIndex > wTotalAnalogue)
        return NULL;
    return pKerAnalogue + wIndex;
}

HKerDigital* HKerStation::findKerDigital(quint16 wIndex)
{
    if(NULL == pKerDigital || wIndex > wTotalDigital)
        return NULL;
    return pKerDigital + wIndex;
}

HKerGroup* HKerStation::findKerGroup(quint16 wIndex)
{
    if(NULL == pKerGroup || wIndex > wTotalGroup)
        return NULL;
    return pKerGroup + wIndex;
}

bool HKerStation::getAttr(quint16 wAttrib,void* pValue,quint32 size)
{
    if(!pValue)
        return false;
    switch(wAttrib)
    {
    case ATTR_ST_NO:          *(ushort*)pValue = station.wStationID;break;
    case ATTR_ST_ADDR:        *(ushort*)pValue = station.wStationAddress;
    case ATTR_ST_NAME:        qstrcpy((char*)pValue,station.szStationName);break;
    case ATTR_ST_SLOCK:       *(uchar*)pValue = station.btStationLock;break;
    case ATTR_ST_TOTALANA:    *(ushort*)pValue = station.wAnalogueCounts;break;
    case ATTR_ST_TOTALDGT:    *(ushort*)pValue = station.wDigitalCounts;break;
    case ATTR_ST_TOTALEGROUP: *(ushort*)pValue = station.wEquipmentGroupCounts;break;
    case ATTR_ST_TOTALLOCK:   *(ushort*)pValue = station.wLockTypeCounts;break;
    default:
        return false;
    }
    return true;
}

bool HKerStation::setAttr(quint16 wAttrib,void* pValue)
{
    if(!pValue)
        return false;
    doSetAttr(wAttrib,pValue);
    return true;
}

bool HKerStation::doSetAttr(quint16 wAttrib,void* pValue)
{
    if(!pValue)
        return false;
    switch(wAttrib)
    {
    case ATTR_ST_NO:          station.wStationID = *(ushort*)pValue;break;
    case ATTR_ST_ADDR:        station.wStationAddress  = *(ushort*)pValue;
    case ATTR_ST_NAME:
    {
        qstrncpy(station.szStationName,(char*)pValue,STATIONNAMELEN-1);
        station.szStationName[STATIONNAMELEN - 1] = 0;
        break;
    }
    case ATTR_ST_SLOCK:       station.btStationLock = *(uchar*)pValue;break;
    default:
        return false;
    }
    return true;
}

TPDIGITAL* HKerStation::findTPDigital(HKerDigital *pKerD)
{
    if(m_pTpDigitalList.count() == 0)
        return NULL;
    TPDIGITAL* TPDigitalTemp = NULL;
    for(int i = 0; i < m_pTpDigitalList.count();i++)
    {
        TPDigitalTemp = (TPDIGITAL*)m_pTpDigitalList[i];
        if(TPDigitalTemp->pDigitalA->getNo() == pKerD->getNo() || TPDigitalTemp->pDigitalB->getNo() == pKerD->getNo())
            break;
    }
    return TPDigitalTemp;
}









