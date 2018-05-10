#include "hkernelhandle.h"
#include "kerstation.h"

HKernelHandle* HKernelHandle::m_pInstance = NULL;
HKernelHandle* HKernelHandle::Instance()
{
    if(!m_pInstance)
    {
        m_pInstance = new HKernelHandle();
        m_pInstance->dbDataInit();
    }

    return m_pInstance;
}


HKernelHandle::HKernelHandle()
{

}

HKernelHandle::~HKernelHandle()
{

}

ushort HKernelHandle::stationIndex2Addr(ushort wStationIndex)
{
    HKerStation* pStation = kerDataBase.findKerStation(wStationIndex);
    if(pStation)
    {
        return pStation->getNo();
    }
    return (ushort)-1;
}

ushort HKernelHandle::stationAddr2Index(ushort wStationAddr)
{
    HKerStation* pStation = kerDataBase.getKerRtu(wStationAddr);
    if(pStation)
        return (pStation - kerDataBase.pKerStation);
    return (ushort)-1;
}
/*
//
bool KERNEL_EXPORT kernelInit()
{
    return true;
}

bool KERNEL_EXPORT kernelEnd()
{
    return true;
}
*/

//启动数据库
bool HKernelHandle::dbDataInit()
{
    if(!kerDataBase.loadData())
        return false;
    return true;
}

//关闭数据
void HKernelHandle::dbDataEnd()
{
    //kerDataBase.saveData();
    HUserDb* pUserDb = kerDataBase.m_pUserDb;
    for(int i = 0; i < kerDataBase.wTotalUserDb;i++,pUserDb++)
    {
        pUserDb->exitProc();
    }
    kerDataBase.saveAnalogue();
    kerDataBase.saveDigital();
    kerDataBase.freeData();
}

//写入数据到数据库
void HKernelHandle::writeRdbData(const char* lpData)
{

}

//获取相关数目
ushort HKernelHandle::getStationNum()
{
    return kerDataBase.wTotalStation;
}

ushort HKernelHandle::getGlossaryGroupNum()
{
    return kerDataBase.wTotalGlossaryGroup;
}

ushort HKernelHandle::getGlossaryNum()
{
    return kerDataBase.wTotalGlossary;
}

//获取当前厂站和测点 信息
DBHANDLE HKernelHandle::getDbHandle(ushort wStation,uchar btType,ushort wNo,uchar btDbType)
{
    DBHANDLE dbHandle;
    dbHandle.pPt = NULL;
    HKerDataBase* kerDb = getKerDBByType(btDbType);
    if(!kerDb)
    {
        dbHandle.pSt= NULL;
        return dbHandle;
    }
    dbHandle.pSt = (HKerStation*)kerDb->getKerStation(wStation);
    if(dbHandle.pSt && btType != TYPE_NULL)
    {
        dbHandle.pPt = ((HKerStation*)dbHandle.pSt)->kerWord(btType,wNo);
        if(dbHandle.pPt == NULL)
            dbHandle.pSt = NULL;
    }
    return dbHandle;
}

//同上
DBHANDLE HKernelHandle::findDbHandle(ushort wStationID,uchar btType,ushort wIndex)
{
    DBHANDLE dbHandle;
    dbHandle.pPt = NULL;
    if(TYPE_NULL == btType)
    {
        dbHandle.pSt = kerDataBase.findKerStation(wIndex);
        return dbHandle;
    }
    dbHandle.pSt = kerDataBase.getKerStation(wStationID);
    if(dbHandle.pSt)
    {
        dbHandle.pPt = (HKerWord*)((HKerStation*)dbHandle.pSt)->findKerWord(btType,wIndex);
        if(!dbHandle.pPt)
            dbHandle.pSt = NULL;
    }
    return dbHandle;
}

//当前厂站测点是否有效
bool HKernelHandle::isValidDbHandle(DBHANDLE dbHandle)
{
    return (dbHandle.pSt != NULL);
}

//人工置数部分
void HKernelHandle::analogueManualSet(DBHANDLE dbHandle,float fValue)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPt || TYPE_ANALOGUE != ((HKerAnalogue*)dbHandle.pPt)->getType())
        return;
    HKerAnalogue* pKerAnalogue = (HKerAnalogue*)dbHandle.pPt;
    pKerAnalogue->manualSet(fValue,(HKerStation*)dbHandle.pSt);
}

void HKernelHandle::digitalManualSet(DBHANDLE dbHandle,uchar btValue)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPt || TYPE_DIGITAL != ((HKerDigital*)dbHandle.pPt)->getType())
        return;
    HKerDigital* pKerDigital = (HKerDigital*)dbHandle.pPt;
    pKerDigital->manualSet(btValue,(HKerStation*)dbHandle.pSt);
}

void HKernelHandle::deManualSet(DBHANDLE dbHandle)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPt)
        return;
    switch(((HKerWord*)dbHandle.pPt)->getType())
    {
    case TYPE_ANALOGUE:
        ((HKerAnalogue*)dbHandle.pPt)->deManualSet((HKerStation*)dbHandle.pSt);
        break;
    case TYPE_DIGITAL:
        ((HKerDigital*)dbHandle.pPt)->deManualSet((HKerStation*)dbHandle.pSt);
        break;
    default:
        break;
    }
}

//更改允许标志
void HKernelHandle::enableFalg(DBHANDLE dbHandle,ushort wFlag,bool bEnable)
{
    if(!isValidDbHandle(dbHandle))
        return;
    ((HKerWord*)dbHandle.pPt)->enableFlag(wFlag,bEnable,(HKerStation*)dbHandle.pSt);
}

//确认信息
void HKernelHandle::ack(DBHANDLE dbHandle)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPt)
        return;
    time_t lTime = time(NULL);
    switch(((HKerWord*)dbHandle.pPt)->getType())
    {
    case TYPE_DIGITAL:
        ((HKerDigital*)dbHandle.pSt)->ack((HKerStation*)dbHandle.pSt,(long)lTime);
        break;
    default:
        break;
    }
}

//获取数据
bool HKernelHandle::getAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue)
{
    if(!isValidDbHandle(dbHandle))
        return false;
    if(!dbHandle.pPt)
        return ((HKerStation*)dbHandle.pSt)->getAttr(wAttrib,pValue,0);
    return ((HKerWord*)dbHandle.pPt)->getAttr(wAttrib,pValue,(HKerStation*)dbHandle.pSt,0);
}

//设置数据
bool HKernelHandle::setAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue)
{
    if(!isValidDbHandle(dbHandle))
        return false;
    if(!dbHandle.pPt)
        return ((HKerStation*)dbHandle.pSt)->setAttr(wAttrib,pValue);
    return ((HKerWord*)dbHandle.pPt)->setAttr(wAttrib,pValue,(HKerStation*)dbHandle.pSt);
}

//写入到实时库
void HKernelHandle::kernelEnterDB(uchar btType,ushort wStationIndex,ushort wPointIndex,void *pData)
{

}

HKerDataBase* HKernelHandle::getKerDBByType(uchar btType)
{
    return NULL;
}


/////////////////////////////////////////////////////////////////接口部分//////////////////////////////////////////////////////////////////////////////////////
ushort KERNEL_EXPORT stationIndex2Addr(ushort wStationIndex)
{
    return HKernelHandle::Instance()->stationIndex2Addr(wStationIndex);
}

ushort KERNEL_EXPORT stationAddr2Index(ushort wStationAddr)
{
    return HKernelHandle::Instance()->stationAddr2Index(wStationAddr);
}
/*
//
bool KERNEL_EXPORT kernelInit()
{
    return true;
}

bool KERNEL_EXPORT kernelEnd()
{
    return true;
}
*/
//启动数据库
bool KERNEL_EXPORT dbDataInit()
{
    return HKernelHandle::Instance()->dbDataInit();
}

//关闭数据
void  KERNEL_EXPORT dbDataEnd()
{
    HKernelHandle::Instance()->dbDataEnd();
}

//写入数据到数据库
void KERNEL_EXPORT writeRdbData(const char* lpData)
{
    HKernelHandle::Instance()->writeRdbData(lpData);
}

//获取相关数目
ushort KERNEL_EXPORT getStationNum()
{
    return HKernelHandle::Instance()->getStationNum();
}

ushort KERNEL_EXPORT getGlossaryGroupNum()
{
    return HKernelHandle::Instance()->getGlossaryGroupNum();
}

ushort KERNEL_EXPORT getGlossaryNum()
{
    return HKernelHandle::Instance()->getGlossaryNum();
}

//获取当前厂站和测点 信息   存疑
DBHANDLE KERNEL_EXPORT getDbHandle(ushort wStation,uchar btType,ushort wNo,uchar btDbType)
{
    return HKernelHandle::Instance()->getDbHandle(wStation,btType,wNo,btDbType);
}

//同上
DBHANDLE KERNEL_EXPORT findDbHandle(ushort wStationID,uchar btType,ushort wIndex)
{
    return HKernelHandle::Instance()->findDbHandle(wStationID,btType,wIndex);
}

//当前厂站测点是否有效
bool KERNEL_EXPORT isValidDbHandle(DBHANDLE dbHandle)
{
    return HKernelHandle::Instance()->isValidDbHandle(dbHandle);
}

//人工置数部分
void KERNEL_EXPORT analogueManualSet(DBHANDLE dbHandle,float fValue)
{
    HKernelHandle::Instance()->analogueManualSet(dbHandle,fValue);
}

void KERNEL_EXPORT digitalManualSet(DBHANDLE dbHandle,uchar btValue)
{
    HKernelHandle::Instance()->digitalManualSet(dbHandle,btValue);
}

void KERNEL_EXPORT deManualSet(DBHANDLE dbHandle)
{
    HKernelHandle::Instance()->deManualSet(dbHandle);
}

//更改允许标志
void KERNEL_EXPORT enableFalg(DBHANDLE dbHandle,ushort wFlag,bool bEnable)
{
    HKernelHandle::Instance()->enableFalg(dbHandle,wFlag,bEnable);
}

//确认信息
void KERNEL_EXPORT ack(DBHANDLE dbHandle)
{
    HKernelHandle::Instance()->ack(dbHandle);
}

//获取数据
bool KERNEL_EXPORT getAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue)
{
    return HKernelHandle::Instance()->getAttr(dbHandle,wAttrib,pValue);
}

//设置数据
bool KERNEL_EXPORT setAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue)
{
    return HKernelHandle::Instance()->setAttr(dbHandle,wAttrib,pValue);
}

//写入到实时库
void KERNEL_EXPORT kernelEnterDB(uchar btType,ushort wStationIndex,ushort wPointIndex,void *pData)
{
    HKernelHandle::Instance()->kernelEnterDB(btType,wStationIndex,wPointIndex,pData);
}
