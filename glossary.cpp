#include "glossary.h"

HGlossary::HGlossary()
{

}

HGlossary::~HGlossary()
{

}

ushort  HGlossary::glossaryGroupID()
{
    return glossary.wGlossaryGroupID;
}

ushort  HGlossary::glossaryID()
{
    return glossary.wGlossaryID;
}

uchar   HGlossary::glossaryType()
{
    return glossary.btGlossaryType;
}

QString HGlossary::glossaryName()
{
    return (QString)glossary.szGlossary;
}


///////////////////////////////////////////////HGlossaryGroup///////////////////////////
HGlossaryGroup::HGlossaryGroup()
{
    wTotalGlossary = 0;
}

HGlossaryGroup::~HGlossaryGroup()
{
    wTotalGlossary = 0;
    if(pGlossary)
    {
        delete[] pGlossary;
        pGlossary = NULL;
    }
}

bool HGlossaryGroup::loadData(FILEHANDLE &fileHandle)
{
    openDB(FILE_TYPE_GLOSSARY);
    wTotalGlossary = glossaryGroup.wGlossaryCounts;
    pGlossary = new GLOSSARY[wTotalGlossary];
    GLOSSARY* glossary = pGlossary;
    for(int i = 0; i < wTotalGlossary;i++,glossary++)
    {
        if(false == loadDBRecord(FILE_TYPE_GLOSSARY,++fileHandle.wGlossary,glossary))
        {
            delete pGlossary;
            return false;
        }
    }
    closeDB(FILE_TYPE_GLOSSARY);
    return true;
}

void HGlossaryGroup::saveData(FILEHANDLE &fileHandle)
{
    //glossaryGroup.wGlossaryCounts = pGlossaryList.count();
    createDB(FILE_TYPE_GLOSSARY);
    GLOSSARY* glossary = pGlossary;
    for(int i = 0; i < wTotalGlossary;i++,glossary++)
    {
        saveDBRecord(FILE_TYPE_GLOSSARY,++fileHandle.wGlossary,glossary);
    }
    closeDB(FILE_TYPE_GLOSSARY);
}

GLOSSARY* HGlossaryGroup::findGlossary(uchar btGlossaryType,ushort wGlossaryID)
{
    if(wTotalGlossary == 0) return NULL;
    GLOSSARY* glossary = pGlossary;
    for(int i = 0; i < wTotalGlossary;i++)
    {
        if(glossary->wGlossaryID = wGlossaryID && glossary->btGlossaryType == btGlossaryType)
            return glossary;
    }
    return NULL;
}

ushort  HGlossaryGroup::glossaryGroupID()
{
    return glossaryGroup.wGlossaryGroupID;
}

QString HGlossaryGroup::glossaryGroupName()
{
    return (QString)glossaryGroup.szGloassaryGroup;
}

uchar HGlossaryGroup::glossaryGroupType()
{
    return glossaryGroup.btGlossaryGroupType;
}
