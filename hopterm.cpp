#include "hopterm.h"

HOpTerm::HOpTerm()
{

}

HOpTerm::~HOpTerm()
{

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
    wTotalOpTerm = 0;
}

HOpTermGroup::~HOpTermGroup()
{
    wTotalOpTerm = 0;
    if(pOpTerm)
    {
        delete[] pOpTerm;
        pOpTerm = NULL;
    }
}

bool HOpTermGroup::loadData(FILEHANDLE &fileHandle)
{
    openDB(FILE_TYPE_OPTERM);
    wTotalOpTerm = opTermGroup.wOpTermCounts;
    pOpTerm = new OPTERM[wTotalOpTerm];
    OPTERM* opTerm = pOpTerm;
    for(int i = 0; i < wTotalOpTerm;i++,opTerm++)
    {
        if(false == loadDBRecord(FILE_TYPE_OPTERM,++fileHandle.wOpTerm,opTerm))
        {
            delete pOpTerm;
            return false;
        }
    }
    closeDB(FILE_TYPE_OPTERM);
    return true;
}

void HOpTermGroup::saveData(FILEHANDLE &fileHandle)
{
    createDB(FILE_TYPE_OPTERM);
    OPTERM* opTerm = pOpTerm;;
    for(int i = 0; i < wTotalOpTerm;i++,opTerm++)
    {
        saveDBRecord(FILE_TYPE_OPTERM,++fileHandle.wOpTerm,opTerm);
    }
    closeDB(FILE_TYPE_OPTERM);
}

OPTERM* HOpTermGroup::findOpTerm(uchar btOpTermType,ushort wOpTermID)
{
    if(wTotalOpTerm == 0) return NULL;
    OPTERM* opTerm = pOpTerm;
    for(int i = 0; i < wTotalOpTerm;i++)
    {
        if(opTerm->wOpTermID = wOpTermID && opTerm->btOpTermType == btOpTermType)
            return opTerm;
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
