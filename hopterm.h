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
    OPTERM* opTermMe();
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

    HOpTerm* findOpTerm(uchar wOpTermType,ushort wOpTermID);

public:
    ushort  opTermNum(){return m_wTotalOpTerm;}
    ushort  opTermGroupID();
    QString opTermGroupName();
    uchar   opTermGroupType();

public:
    OPTERMGROUP opTermGroup;
private:
    ushort m_wTotalOpTerm;
    HOpTerm* m_pOpTerm;

};

#endif // HOPTERM_H
