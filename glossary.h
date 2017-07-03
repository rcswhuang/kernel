#ifndef HGLOSSARY_H
#define HGLOSSARY_H

/*
 * 测点操作术语类
*/
#include "kernelapi.h"
#include <QList>

class HGlossary
{
public:
    HGlossary();
    ~HGlossary();
public:
    ushort  glossaryGroupID();
    ushort  glossaryID();
    uchar   glossaryType();
    QString glossaryName();
private:
    GLOSSARY glossary;
};


class HGlossaryGroup
{
public:
    HGlossaryGroup();
    ~HGlossaryGroup();
public:
    bool loadData(FILEHANDLE &fileHandle);
    void saveData(FILEHANDLE &FILEHANDLE);

    GLOSSARY* findGlossary(uchar wGlossaryType,ushort wGlossaryID);

public:
    ushort  glossaryNum(){return wTotalGlossary;}
    ushort  glossaryGroupID();
    QString glossaryGroupName();
    uchar   glossaryGroupType();

public:
    GLOSSARYGROUP glossaryGroup;
private:
    ushort wTotalGlossary;
    //QList<GLOSSARY*> pGlossaryList;
    GLOSSARY* pGlossary;

};

#endif // HGLOSSARY_H
