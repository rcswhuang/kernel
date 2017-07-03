#ifndef HKERWORD_H
#define HKERWORD_H

#include <QObject>
class HKerStation;
class HKerWord : public QObject
{
public:
    HKerWord();
    ~HKerWord();
public:
    virtual quint16 getNo() = 0;
    virtual quint8 getType() = 0;

    virtual void doEnableFlag(int wFlag,bool bEnable,HKerStation* pStation);
    virtual void enableFlag(int wFlag,bool bEnable,HKerStation* pStation);

    virtual bool getAttr(quint16 wAttrib,void* pValue,HKerStation* pStation,quint32 size = 0) = 0;
    virtual bool setAttr(quint16 wAttrib,void* pValue,HKerStation* pStation) = 0;
    virtual bool doSetAttr(quint16 wAttrib,void* pValue,HKerStation* pStation) = 0;

};

#endif // HKERWORD_H
