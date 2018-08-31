#include "hopterm.h"

HOpTerm::HOpTerm()
{

}

HOpTerm::~HOpTerm()
{

}
OPTERM* HOpTerm::opTermMe()
{
    return &opTerm;
}

ushort  HOpTerm::opTermGroupID()
{
    return opTerm.wOpTermGroupID;
}

ushort  HOpTerm::opTermID()
{
    return opTerm.wOpTermID;
}

uchar   HOpTerm::opTermType()
{
    return opTerm.btOpTermType;
}

QString HOpTerm::opTermName()
{
    return (QString)opTerm.szOpTerm;
}


///////////////////////////////////////////////HOpTermGroup///////////////////////////
HOpTermGroup::HOpTermGroup()
{
    m_wTotalOpTerm = 0;
}

HOpTermGroup::~HOpTermGroup()
{
    m_wTotalOpTerm = 0;
    if(m_pOpTerm)
    {
        delete[] m_pOpTerm;
        m_pOpTerm = NULL;
    }
}

bool HOpTermGroup::loadData(FILEHANDLE &fileHandle)
{
    int fd = openDB(FILE_TYPE_OPTERM);
    m_wTotalOpTerm = opTermGroup.wOpTermCounts;
    m_pOpTerm = new HOpTerm[m_wTotalOpTerm];
    HOpTerm* pOpTerm = m_pOpTerm;
    for(int i = 0; i < m_wTotalOpTerm;i++,pOpTerm++)
    {
        if(false == loadDBRecord(fd,++fileHandle.wOpTerm,pOpTerm->opTermMe()))
        {
            delete pOpTerm;
            continue;
        }
    }
    closeDB(FILE_TYPE_OPTERM);
    return true;
}

void HOpTermGroup::saveData(FILEHANDLE &fileHandle)
{
    int fd = createDB(FILE_TYPE_OPTERM);
    HOpTerm* pOpTerm = m_pOpTerm;;
    for(int i = 0; i < m_wTotalOpTerm;i++,pOpTerm++)
    {
        saveDBRecord(fd,++fileHandle.wOpTerm,pOpTerm->opTermMe());
    }
    closeDB(FILE_TYPE_OPTERM);
}

HOpTerm* HOpTermGroup::findOpTerm(uchar btOpTermType,ushort wOpTermID)
{
    if(m_wTotalOpTerm == 0) return NULL;
    HOpTerm* pOpTerm = m_pOpTerm;
    for(int i = 0; i < m_wTotalOpTerm;i++)
    {
        if(pOpTerm->opTermID() == wOpTermID && pOpTerm->opTermType() == btOpTermType)
            return pOpTerm;
    }
    return NULL;
}

ushort  HOpTermGroup::opTermGroupID()
{
    return opTermGroup.wOpTermGroupID;
}

QString HOpTermGroup::opTermGroupName()
{
    return (QString)opTermGroup.szOpTermGroup;
}

uchar HOpTermGroup::opTermGroupType()
{
    return opTermGroup.btOpTermGroupType;
}
