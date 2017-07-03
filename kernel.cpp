#include "kernel.h"
#include "kernelapi.h"
#include"kerdatabase.h"
#include"kerstation.h"
#include "kerword.h"
#include "keranalogue.h"
#include "kerdigital.h"
#include "time.h"
#include "userdb.h"

Kernel::Kernel()
{
}




/////////////////////////////////////////////////全局函数定义//////////////////////////////////////////////
HKerDataBase kerDataBase;


/////////////////////////////////////////////////////对外接口////////////////////////////////////////////


ushort KERNEL_EXPORT stationIndex2Addr(ushort wStationIndex)
{
    HKerStation* pStation = kerDataBase.findKerStation(wStationIndex);
    if(pStation)
    {
        return pStation->getNo();
    }
    return (ushort)-1;
}

ushort KERNEL_EXPORT stationAddr2Index(ushort wStationAddr)
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
bool KERNEL_EXPORT dbDataInit()
{
    if(!kerDataBase.loadData())
        return false;
    return true;
}

//关闭数据
void  KERNEL_EXPORT dbDataEnd()
{
    //kerDataBase.saveData();
    HUserDb* pUserDb = kerDataBase.pUserDb;
    for(int i = 0; i < kerDataBase.wTotalUserDb;i++,pUserDb++)
    {
        pUserDb->exitLibrary();
    }


    kerDataBase.saveAnalogue();
    kerDataBase.saveDigital();
    kerDataBase.freeData();
}

//写入数据到数据库
void KERNEL_EXPORT writeRdbData(const char* lpData)
{

}

//获取相关数目
ushort KERNEL_EXPORT getStationNum()
{
    return kerDataBase.wTotalStation;
}

ushort KERNEL_EXPORT getGlossaryGroupNum()
{
    return kerDataBase.wTotalGlossaryGroup;
}

ushort KERNEL_EXPORT getGlossaryNum()
{
    return kerDataBase.wTotalGlossary;
}


//获取当前厂站和测点 信息   存疑
DBHANDLE KERNEL_EXPORT getDbHandle(ushort wStation,uchar btType,ushort wNo)
{
    DBHANDLE dbHandle;
    dbHandle.pPT = NULL;
    return dbHandle;
}

//同上
DBHANDLE KERNEL_EXPORT findDbHandle(ushort wStationID,uchar btType,ushort wIndex)
{
    DBHANDLE dbHandle;
    dbHandle.pPT = NULL;
    if(TYPE_NULL == btType)
    {
        dbHandle.pST = kerDataBase.findKerStation(wIndex);
        return dbHandle;
    }
    dbHandle.pST = kerDataBase.getKerStation(wStationID);
    if(dbHandle.pST)
    {
        dbHandle.pST = ((HKerStation*)dbHandle.pST)->findKerWord(btType,wIndex);
        if(!dbHandle.pST)
            dbHandle.pST = NULL;
    }
    return dbHandle;
}

//当前厂站测点是否有效
bool KERNEL_EXPORT isValidDbHandle(DBHANDLE dbHandle)
{
    return (dbHandle.pST != NULL);
}


//人工置数部分
void KERNEL_EXPORT analogueManualSet(DBHANDLE dbHandle,float fValue)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPT || TYPE_ANALOGUE != ((HKerAnalogue*)dbHandle.pPT)->getType())
        return;
    HKerAnalogue* pKerAnalogue = (HKerAnalogue*)dbHandle.pPT;
    pKerAnalogue->manualSet(fValue,(HKerStation*)dbHandle.pST);
}

void KERNEL_EXPORT digitalManualSet(DBHANDLE dbHandle,uchar btValue)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPT || TYPE_DIGITAL != ((HKerDigital*)dbHandle.pPT)->getType())
        return;
    HKerDigital* pKerDigital = (HKerDigital*)dbHandle.pPT;
    pKerDigital->manualSet(btValue,(HKerStation*)dbHandle.pST);
}

void KERNEL_EXPORT deManualSet(DBHANDLE dbHandle)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPT)
        return;
    switch(((HKerWord*)dbHandle.pPT)->getType())
    {
    case TYPE_ANALOGUE:
        ((HKerAnalogue*)dbHandle.pPT)->deManualSet((HKerStation*)dbHandle.pST);
        break;
    case TYPE_DIGITAL:
        ((HKerDigital*)dbHandle.pPT)->deManualSet((HKerStation*)dbHandle.pST);
        break;
    default:
        break;
    }

}


//更改允许标志
void KERNEL_EXPORT enableFalg(DBHANDLE dbHandle,ushort wFlag,bool bEnable)
{
    if(!isValidDbHandle(dbHandle))
        return;
    ((HKerWord*)dbHandle.pPT)->enableFlag(wFlag,bEnable,(HKerStation*)dbHandle.pST);
}

//确认信息
void KERNEL_EXPORT ack(DBHANDLE dbHandle)
{
    if(!isValidDbHandle(dbHandle) || NULL == dbHandle.pPT)
        return;
    time_t lTime = time(NULL);
    switch(((HKerWord*)dbHandle.pPT)->getType())
    {
    case TYPE_DIGITAL:
        ((HKerDigital*)dbHandle.pST)->ack((HKerStation*)dbHandle.pST,(long)lTime);
        break;
    default:
        break;
    }
}

//获取数据

bool KERNEL_EXPORT getAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue)
{
    if(!isValidDbHandle(dbHandle))
        return false;
    if(!dbHandle.pPT)
        return ((HKerStation*)dbHandle.pST)->getAttr(wAttrib,pValue,0);
    return ((HKerWord*)dbHandle.pPT)->getAttr(wAttrib,pValue,(HKerStation*)dbHandle.pST,0);
}

//设置数据
bool KERNEL_EXPORT setAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue)
{
    if(!isValidDbHandle(dbHandle))
        return false;
    if(!dbHandle.pPT)
        return ((HKerStation*)dbHandle.pST)->setAttr(wAttrib,pValue);
    return ((HKerWord*)dbHandle.pPT)->setAttr(wAttrib,pValue,(HKerStation*)dbHandle.pST);
}

//写入到实时库
void KERNEL_EXPORT kernelEnterDB(uchar btType,ushort wStationIndex,ushort wPointIndex,void *pData)
{

}

