#include "kerdatabase.h"
#include "kernelapi.h"
#include "pointterm.h"
#include "locktype.h"
#include "glossary.h"
#include <QDir>

HKerDataBase::HKerDataBase(QObject *parent) : QObject(parent)
{
    wTotalStation = 0;
    pKerStation = NULL;
    dsTimer = NULL;
    dsTimer = new QTimer(this);
    //connect(dsTimer,SIGNAL(timeout()),this,SLOT(dsDigitalUpdate()));
    //dsTimer->start(1000);
}

HKerDataBase::~HKerDataBase()
{
    freeData();
}

void HKerDataBase::freeData()
{
    wTotalStation = 0;
    if(pKerStation)
    {
        delete[] pKerStation;
        pKerStation = NULL;
    }

    if(dsTimer)
    {
        dsTimer->stop();
        delete dsTimer;
    }

    wTotalPointTerm = 0;
    if(pPointTerm)
    {
        delete []pPointTerm;
        pPointTerm = NULL;
    }

    wTotalGlossaryGroup = 0;
    if(pGlossaryGroup)
    {
        delete [] pGlossaryGroup;
        pGlossaryGroup = NULL;
    }

    wTotalUserDb = 0;
    if(pUserDb)
    {
        delete [] pUserDb;
        pUserDb = NULL;
    }

}

//数据相关操作
bool HKerDataBase::loadData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    DATAFILEHEADER dataFileHandle;

    //测点类型
    openDB(FILE_TYPE_POINTTERM);
    loadDataFileHeader(FILE_TYPE_POINTTERM,&dataFileHandle);
    wTotalPointTerm = dataFileHandle.wTotal;
    pPointTerm = new HPointTerm[wTotalPointTerm];
    HPointTerm* pointTerm = pPointTerm;
    for(int i = 0; i < wTotalPointTerm;i++,pointTerm++)
    {
        if(false == loadDBRecord(FILE_TYPE_POINTTERM,++fileHandle.wPointTerm,&pointTerm->pointTerm))
        {
            delete[] pPointTerm;
            pPointTerm = NULL;
            break;
        }
    }

    //操作术语
    openDB(FILE_TYPE_GLOSSARYGROUP);
    loadDataFileHeader(FILE_TYPE_GLOSSARYGROUP,&dataFileHandle);
    wTotalGlossaryGroup = dataFileHandle.wTotal;
    pGlossaryGroup = new HGlossaryGroup[wTotalGlossaryGroup];
    HGlossaryGroup* glossaryGroup = pGlossaryGroup;
    for(int i = 0; i < wTotalGlossaryGroup;i++,glossaryGroup++)
    {
        if(false == loadDBRecord(FILE_TYPE_GLOSSARYGROUP,++fileHandle.wGlossaryGroup,&glossaryGroup->glossaryGroup))
        {
            delete[] pGlossaryGroup;
            pGlossaryGroup = NULL;
            break;
        }
        if(false == pGlossaryGroup->loadData(fileHandle))
        {
            delete[] pGlossaryGroup;
            pGlossaryGroup = NULL;
            break;
        }
    }
    /* //需要增加默认操作术语吗？
    bool bfind = false;
    for(int i = 0; i < m_pGlossaryGroupList.count();i++)
    {
        HGlossaryGroup* pGlossaryGroup = (HGlossaryGroup*)m_pGlossaryGroupList[i];
        if(pGlossaryGroup->glossaryGroup.btGlossaryGroupType == TYPE_POINT_DEFAULT)
        {
            bfind = true;
            break;
        }
    }
    if(!bfind)
    {
        //addGlossaryGroup(TYPE_POINT_DEFAULT);
    }*/

    //锁类型
    openDB(FILE_TYPE_LOCKTYPE);
    wTotalLockType = dataFileHandle.wTotal;
    pLockType = new HLockType[wTotalLockType];
    HLockType* lockType = pLockType;
    for(int i = 0; i < wTotalLockType;i++)
    {
        if(false == loadDBRecord(FILE_TYPE_LOCKTYPE,++fileHandle.wLockType,&lockType->wfLockType))
        {
            delete[] pLockType;
            pLockType = NULL;
            break;
        }
    }

    //厂站信息
    openDB(FILE_TYPE_STATION);
    loadDataFileHeader(FILE_TYPE_STATION,&dataFileHandle);
    wTotalStation = dataFileHandle.wTotal;
    pKerStation = new HKerStation[wTotalStation];
    HKerStation *pKerS = pKerStation;
    for(int i = 0 ; i < dataFileHandle.wTotal; i++,pKerS++)
    {
        if (false == loadDBRecord(FILE_TYPE_STATION, ++fileHandle.wStation, &pKerS->station ) )
        {
            delete[] pKerS;
            break;
        }
        if(false == pKerS->loadData(fileHandle))
        {
            delete [] pKerS;
            break;
        }
    }

    closeDB(FILE_TYPE_POINTTERM);
    closeDB(FILE_TYPE_GLOSSARYGROUP);
    closeDB(FILE_TYPE_LOCKTYPE);
    closeDB(FILE_TYPE_STATION);




    //加载插件代码  用 MACRO这个来代替  加载Qt编译出来的动态库
    char szPluginPath[128];
    getDataFilePath(DFPATH_PLUGIN,szPluginPath);
    QString strPluginName = QString(szPluginPath);
    QDir dir(strPluginName);
    if(dir.exists())
    {
        QStringList strPluginList = dir.entryList(QDir::Files);
        pUserDb = new HUserDb[strPluginList.count()];
        wTotalUserDb = strPluginList.count();
        HUserDb* userDb = pUserDb;
        for(int i = 0; i< strPluginList.count();i++,userDb++)
        {
            strPluginName += strPluginList[i];
            if(!QLibrary::isLibrary(strPluginName))
            {
                delete (pUserDb+wTotalUserDb);//删掉最后一个空间就可以了
                wTotalUserDb--;
            }
            QLibrary library(strPluginName);
            if(library.load())
            {
                USERDBPROC userDbProc = (USERDBPROC)library.resolve(UDBCALLBACKPROCNAME);
                if(!userDbProc)
                    library.unload();
                else
                {
                    userDb->strUserDBName = strPluginName;
                    userDb->userdbProc = userDbProc;
                    userDb->initLibrary();
                }
            }
        }
    }

    return true;
}

//需要这个功能吗？实时库退出时，应该不需要把相关信息写入dat文件，只要把当前值保留下来就可以了。
void HKerDataBase::saveData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));

    //厂站信息
    openDB(FILE_TYPE_STATION);
    HKerStation* pKerS = pKerStation;
    for(int i = 0; i < wTotalStation;i++,pKerS++)
    {
        pKerS->saveData(fileHandle);
        saveDBRecord(FILE_TYPE_STATION,++fileHandle.wStation,&pKerS->station);
    }
    closeDB(FILE_TYPE_STATION);
}



bool HKerDataBase::loadAnalogue()
{
    return false;
}

void HKerDataBase::saveAnalogue()
{

}

bool HKerDataBase::loadDigital()
{
    return false;
}

void HKerDataBase::saveDigital()
{

}


//实时数据库厂站操作
quint16 HKerDataBase::getStationNum()
{
    return wTotalStation;
}

quint16 HKerDataBase::getStationIndex(HKerStation* pKerS)
{
    if(pKerStation == NULL || !pKerS) return (quint16)-1;

    return (quint16)(pKerS - pKerStation);
}

HKerStation* HKerDataBase::getKerRtu(quint16 wAddr)
{
    if(pKerStation == NULL)
        return NULL;
    HKerStation* pKerS = pKerStation;
    for(int i = 0; i < wTotalStation;i++,pKerS++)
    {
        if(pKerS->getRtuNo() == wAddr)
            return pKerS;
    }
    return NULL;
}

HKerStation* HKerDataBase::getKerStation(quint16 wNo)
{
    if(pKerStation == NULL)
        return NULL;
    HKerStation* pKerS = pKerStation;
    for(int i = 0; i < wTotalStation;i++,pKerS++)
    {
        if(pKerS->getNo() == wNo)
            return pKerS;
    }
    return NULL;
}

HKerStation* HKerDataBase::findKerStation(quint16 wIndex)
{
    if(pKerStation == NULL || wIndex > wTotalStation)
        return NULL;
    return pKerStation + wIndex;
}

HKerWord* HKerDataBase::getKerWord(quint16 wStationNo,quint8 btType,quint16 wNo)
{
    HKerStation * pKerS = getKerStation(wStationNo);
    if(pKerS)
    {
        return pKerS->kerWord(btType,wNo);
    }
    return NULL;
}

HKerWord* HKerDataBase::findKerWord(quint16 wStationNo,quint8 btType,quint16 wIndex)
{
    HKerStation * pKerS = getKerStation(wStationNo);
    if(pKerS)
    {
        return pKerS->findKerWord(btType,wIndex);
    }
    return NULL;
}

//锁类型
HLockType* HKerDataBase::findLockType(ushort wStationID,QString strLockType)
{
    if(pLockType == NULL) return NULL;
    HLockType* pLT = pLockType;
    for(int i = 0; i < wTotalLockType; i++,pLT++)
    {
        if(pLT->stationID() == wStationID && strLockType == QString(pLT->lockType()))
            return pLT;
    }
    return NULL;
}

HLockType* HKerDataBase::findLockType(ushort wStationID,ushort wLockTypeID)
{
    if(pLockType == NULL) return NULL;
    HLockType* pLT = pLockType;
    for(int i = 0; i < wTotalLockType; i++,pLT++)
    {
        if(pLT->stationID() == wStationID && wLockTypeID == pLT->lockTypeID())
            return pLT;
    }
    return NULL;
}

//测点类型
HPointTerm* HKerDataBase::findPointTerm(ushort wPointTermID)
{
    if(pPointTerm == NULL) return NULL;
    HPointTerm* pPointTerm1 = pPointTerm;
    for(int i = 0; i < wTotalPointTerm; i++,pPointTerm1++)
    {
        if(pPointTerm1->pointTermID() == wPointTermID)
            return pPointTerm1;
    }
    return NULL;
}

HPointTerm* HKerDataBase::findPointTerm(QString strPointTerm)
{
    if(pPointTerm == NULL) return NULL;
    HPointTerm* pPointTerm1 = pPointTerm;
    for(int i = 0; i < wTotalPointTerm; i++,pPointTerm1++)
    {
        if(pPointTerm1->pointTermName() == strPointTerm)
            return pPointTerm1;
    }
    return NULL;
}

//操作术语
ushort HKerDataBase::getGlossaryGroupNum()
{
    return wTotalGlossaryGroup;
}

ushort HKerDataBase::getGlossaryNum()
{
    if(pGlossaryGroup == NULL)
        return 0;
    ushort wGlossaryNum = 0;
    HGlossaryGroup* glossaryGroup = pGlossaryGroup;
    for(int i = 0; i < wTotalGlossaryGroup;i++,glossaryGroup++)
    {
        wGlossaryNum += glossaryGroup->glossaryNum();
    }
    return wGlossaryNum;
}

HGlossaryGroup* HKerDataBase::getGlossaryGroup(ushort wNo)
{
    if(pGlossaryGroup == NULL)
        return NULL;
    HGlossaryGroup* glossaryGroup = pGlossaryGroup;
    for(int i = 0; i < wTotalGlossaryGroup;i++,glossaryGroup++)
    {
        if(glossaryGroup->glossaryGroupID() == wNo)
            return glossaryGroup;
    }
    return NULL;
}

GLOSSARY* HKerDataBase::getGlossary(ushort wGlossaryGroupID,uchar btType,ushort wGlossaryID)
{
    HGlossaryGroup* glossaryGroup = getGlossaryGroup(wGlossaryGroupID);
    if(glossaryGroup)
    {
        return glossaryGroup->findGlossary(btType,wGlossaryID);
    }
    return NULL;
}


//超过时间将按照实际值计算
void HKerDataBase::dsDigitalUpdate()
{
    static int tpTime = 10;

    HKerStation* pStation = pKerStation;
    if(pKerStation)
    {
        for(int i = 0; i< wTotalStation;i++,pStation++)
        {
            for(int j = 0; j < pStation->m_pTpDigitalList.count();j++)
            {
                TPDIGITAL* tpDigital = pStation->m_pTpDigitalList[j];
                if(tpDigital && (tpDigital->btState != TP_NORMAL) && (++tpDigital->wWaitCount > tpTime))
                {
                    tpDigital->btState = TP_NORMAL;
                    tpDigital->wWaitCount = 0;
                }
            }
        }
    }
}



