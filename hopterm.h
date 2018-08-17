#ifndef HOPTERM_H
#define HOPTERM_H

/*
 * 测点操作术语类
*/
#include "hkernelapi.h"
#include "publicdata.h"
#include <QList>

class HOpTerm
{
public:
    HOpTerm();
    ~HOpTerm();
public:
    ushort  opTermGroupID();
    ushort  opTermID();
    uchar   opTermType();
    QString opTermName();
private:
    OPTERM opTerm;
};


class HOpTermGroup
{
public:
    HOpTermGroup();
    ~HOpTermGroup();
public:
    bool loadData(FILEHANDLE &fileHandle);
    void saveData(FILEHANDLE &fileHandle);

    OPTERM* findOpTerm(uchar wOpTermType,ushort wOpTermID);

public:
    ushort  opTermNum(){return wTotalOpTerm;}
    ushort  opTermGroupID();
    QString opTermGroupName();
    uchar   opTermGroupType();

public:
    OPTERMGROUP opTermGroup;
private:
    ushort wTotalOpTerm;
    OPTERM* pOpTerm;

};

#endif // HOPTERM_H
