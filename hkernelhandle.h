#ifndef HKERNELHANDLE_H
#define HKERNELHANDLE_H

#include "kernel.h"
#include "kernelapi.h"
#include"kerdatabase.h"
#include"kerstation.h"
#include "kerword.h"
#include "keranalogue.h"
#include "kerdigital.h"
#include "time.h"
#include "userdb.h"
#include "hkernelapi.h"
#include <QList>

/////////////////////////////////////////////////全局函数定义//////////////////////////////////////////////


/*
HKerDataBase* getKerDBByType(uchar btType)
{
    if(TYPE_DB_REALTIME == btType)
        return &kerDataBase;
}*/


class HKernelHandle
{
public:
    static HKernelHandle* Instance();
    static HKernelHandle* m_pInstance;
    HKernelHandle();
    ~HKernelHandle();

    //接口函数
public:
    bool dbDataInit();

    //关闭数据
    void dbDataEnd();

    //写入数据到数据库
    void writeRdbData(const char* lpData);

    //获取相关数目
    ushort getStationNum();

    ushort getGlossaryGroupNum();
    ushort getGlossaryNum();

    //获取当前厂站和测点
    DBHANDLE getDbHandle(ushort wStation,uchar btType,ushort wNo,uchar btDBType);

    //同上
    DBHANDLE findDbHandle(ushort wStationID,uchar btType,ushort wIndex);

    //当前厂站测点是否有效
    bool isValidDbHandle(DBHANDLE dbHandle);


    //人工置数部分
    void analogueManualSet(DBHANDLE dbHandle,float fValue);

    void digitalManualSet(DBHANDLE dbHandle,uchar btValue);

    void deManualSet(DBHANDLE dbHandle);


    //更改允许标志
    void enableFalg(DBHANDLE dbHandle,ushort wFlag,bool bEnable);

    //确认信息
    void ack(DBHANDLE dbHandle);

    //获取数据
    bool getAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue);

    //设置数据
    bool setAttr(DBHANDLE dbHandle,ushort wAttrib,void* pValue);

    //写入到实时库
    void kernelEnterDB(uchar btType,ushort wStationIndex,ushort wPointIndex,void *pData);

    ushort stationIndex2Addr(ushort wStationIndex);

    ushort stationAddr2Index(ushort wStationAddr);


    //内部使用函数
public:
    HKerDataBase* getKerDBByType(uchar btType);

public:
    HKerDataBase kerDataBase;
    QList<HKerDataBase*> m_tempKerDBList; //临时数据库

};

#endif // HKERNELHANDLE_H
