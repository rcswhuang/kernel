#include "hkerdatabase.h"
#include "hkernelapi.h"
#include "hpointterm.h"
#include "hlocktype.h"
#include "hopterm.h"
#include <QDir>

HKerDataBase::HKerDataBase(QObject *parent) : QObject(parent)
{
    wTotalStation = 0;
    pKerStation = NULL;
    dsTimer = NULL;
    dsTimer = new QTimer(this);
    m_pUserDb = NULL;
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

    wTotalOpTermGroup = 0;
    if(pOpTermGroup)
    {
        delete [] pOpTermGroup;
        pOpTermGroup = NULL;
    }

    wTotalUserDb = 0;
    if(m_pUserDb)
    {
        delete []m_pUserDb;
        m_pUserDb = NULL;
    }

}

//数据相关操作
bool HKerDataBase::loadData()
{
    FILEHANDLE fileHandle;
    memset(&fileHandle,0,sizeof(FILEHANDLE));
    DATAFILEHEADER dataFileHandle;

    //测点类型
    int fd = openDB(FILE_TYPE_POINTTERM);
    loadDataFileHeader(fd,&dataFileHandle);
    wTotalPointTerm = dataFileHandle.wTotal;
    pPointTerm = new HPointTerm[wTotalPointTerm];
    HPointTerm* pointTerm = pPointTerm;
    for(int i = 0; i < wTotalPointTerm;i++,pointTerm++)
    {
        if(false == loadDBRecord(fd,++fileHandle.wPointType,&pointTerm->pointTerm))
        {
            delete[] pPointTerm;
            pPointTerm = NULL;
            continue;
        }
    }

    //操作术语
    fd = openDB(FILE_TYPE_OPTERMGROUP);
    loadDataFileHeader(fd,&dataFileHandle);
    wTotalOpTermGroup = dataFileHandle.wTotal;
    pOpTermGroup = new HOpTermGroup[wTotalOpTermGroup];
    HOpTermGroup* opTermGroup = pOpTermGroup;
    for(int i = 0; i < wTotalOpTermGroup;i++,opTermGroup++)
    {
        if(false == loadDBRecord(fd,++fileHandle.wOpTermGroup,&opTermGroup->opTermGroup))
        {
            delete[] pOpTermGroup;
            pOpTermGroup = NULL;
            continue;
        }
        if(false == pOpTermGroup->loadData(fileHandle))
        {
            delete[] pOpTermGroup;
            pOpTermGroup = NULL;
            continue;
        }
    }

    //锁类型
    fd = openDB(FILE_TYPE_LOCKTYPE);
    loadDataFileHeader(fd,&dataFileHandle);
    wTotalLockType = dataFileHandle.wTotal;
    pLockType = new HLockType[wTotalLockType];
    HLockType* lockType = pLockType;
    for(int i = 0; i < wTotalLockType;i++)
    {
        if(false == loadDBRecord(fd,++fileHandle.wLockType,&lockType->wfLockType))
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
            delete[ ]pKerS;
            break;
        }
    }

    closeDB(FILE_TYPE_POINTTERM);
    closeDB(FILE_TYPE_OPTERMGROUP);
    closeDB(FILE_TYPE_LOCKTYPE);
    closeDB(FILE_TYPE_STATION);




    //加载插件代码  用 MACRO这个来代替  加载Qt编译出来的动态库
    char szPluginPath[128];
    getDataFilePath(DFPATH_PLUGIN,szPluginPath);//已经是d:/wf/plugin文件夹
    QString strPluginName = QString(szPluginPath) + "/" + "run";//防止在d:/wf/plugin/run文件夹里面的dll才加载
    QDir dir(strPluginName);
    if(dir.exists())
    {
        QStringList strPluginList = dir.entryList(QDir::Files);
        wTotalUserDb = strPluginList.count();
        m_pUserDb = new HUserDb[wTotalUserDb];
        HUserDb* userDb = m_pUserDb;
        for(int i = 0; i< strPluginList.count();i++,userDb++)
        {
            strPluginName += strPluginList[i];
            if(!QLibrary::isLibrary(strPluginName))
            {
                delete (m_pUserDb+wTotalUserDb);//删掉最后一个空间就可以了
                wTotalUserDb--;
                userDb--;
                continue;
            }
            QLibrary library(strPluginName);
            if(library.load())
            {
                PLUGINPROC pluginProc = (PLUGINPROC)library.resolve("pluginProc");
                if(!pluginProc)
                    library.unload();
                else
                {
                    //userDb->strUserDBName = strPluginName;
                    //userDb->pluginProc = pluginProc;
                    userDb->initProc();
                }
            }
        }
        if(m_pUserDb)
        {
            for(int i = 0;i < wTotalUserDb;i++)
            {
                HUserDb* pUserDb = (HUserDb*)(m_pUserDb+i);
                if(!pUserDb) continue;
                pUserDb->loadData();
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
ushort HKerDataBase::getOpTermGroupNum()
{
    return wTotalOpTermGroup;
}

ushort HKerDataBase::getOpTermNum()
{
    if(pOpTermGroup == NULL)
        return 0;
    ushort wOpTermNum = 0;
    HOpTermGroup* opTermGroup = pOpTermGroup;
    for(int i = 0; i < wTotalOpTermGroup;i++,opTermGroup++)
    {
        wOpTermNum += opTermGroup->opTermNum();
    }
    return wOpTermNum;
}

HOpTermGroup* HKerDataBase::getOpTermGroup(ushort wNo)
{
    if(pOpTermGroup == NULL)
        return NULL;
    HOpTermGroup* opTermGroup = pOpTermGroup;
    for(int i = 0; i < wTotalOpTermGroup;i++,opTermGroup++)
    {
        if(opTermGroup->opTermGroupID() == wNo)
            return opTermGroup;
    }
    return NULL;
}

HOpTerm* HKerDataBase::getOpTerm(ushort wOpTermGroupID,uchar btType,ushort wOpTermID)
{
    HOpTermGroup* opTermGroup = getOpTermGroup(wOpTermGroupID);
    if(opTermGroup)
    {
        return opTermGroup->findOpTerm(btType,wOpTermID);
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



