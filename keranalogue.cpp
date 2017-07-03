#include "keranalogue.h"
#include "time.h"
#include "publicdata.h"
HKerAnalogue::HKerAnalogue()
{

}

HKerAnalogue::~HKerAnalogue()
{

}

bool HKerAnalogue::isResult(quint16 wResult)
{
    return analogue.wRelDigitalID & wResult;
}

void HKerAnalogue::setResult(quint16 wResult)
{
    analogue.wRelDigitalID |= wResult;
}

void HKerAnalogue::deSetResult(quint16 wResult)
{
    analogue.wRelDigitalID &= ~wResult;
}

void HKerAnalogue::doWrite(float fValue,HKerStation* pStation,long lTime)
{
    checkValue(fValue,pStation,lTime);
}

void HKerAnalogue::checkValue(float fValue,HKerStation* pStation,long lTime)
{
    analogue.fValue = fValue;
}

bool HKerAnalogue::writeValue(float fValue,HKerStation* pStation,long lTime)
{
    if(isResult(RESULT_MANUAL))
        return false;
    doWrite(fValue,pStation,lTime);
    return true;
}

//人工置数等信息
void HKerAnalogue::doManualSet(float fValue,HKerStation* pStation)
{
    setResult(RESULT_MANUAL);
    time_t lTime = time(NULL);
    doWrite(fValue,pStation,(long)lTime);
}

void HKerAnalogue::manualSet(float fValue,HKerStation* pStation)
{
    doManualSet(fValue,pStation);
}

void HKerAnalogue::doDeManualSet(HKerStation* pStation)
{
    deSetResult(RESULT_MANUAL);
}

void HKerAnalogue::deManualSet(HKerStation* pStation)
{
    if(!isResult(RESULT_MANUAL))
        return;
    doDeManualSet(pStation);
}

//自身属性获取
quint16 HKerAnalogue::getNo()
{
    return analogue.wAnalogueID;
}

quint8 HKerAnalogue::getType()
{
    return TYPE_ANALOGUE;
}

quint16 HKerAnalogue::getIndex(HKerStation* pStation)
{
    return 0;
}

const char* HKerAnalogue::getName()
{
    return analogue.szAnalogueOriginalName;
}

//设置实时值和获取实时值
bool HKerAnalogue::getAttr(quint16 wAttrib, void *pValue, HKerStation *pStation , quint32 size)
{
    if(!pValue)
        return false;
    switch(wAttrib)
    {
    case ATTR_ANA_SNO :*(ushort*)pValue = analogue.wStationID;break;
    case ATTR_ANA_NO  :  *(ushort*)pValue = analogue.wStationID;break;
    case ATTR_ANA_NAME :
    {
        qstrcpy((char*)pValue,analogue.szAnalogueName);
        break;
    }
    case ATTR_ANA_ORINAME:
    {
        qstrcpy((char*)pValue,analogue.szAnalogueOriginalName);
        break;
    }
    case ATTR_ANA_TYPE:*(int*)pValue = analogue.btAnalogueType;break;
    case ATTR_ANA_UINT:*(int*)pValue = analogue.btUint;break;
    case ATTR_ANA_CC1:*(float*)pValue = analogue.fCC1;break;
    case ATTR_ANA_CC2:*(float*)pValue = analogue.fCC2;break;
    case ATTR_ANA_CC3 :*(float*)pValue = analogue.fCC3;break;
    case ATTR_ANA_DIFF:*(float*)pValue = analogue.fDifference;break;
    case ATTR_ANA_GRADE:*(int*)pValue = analogue.nPowerGrade;break;
    case ATTR_ANA_SENDFLAG:*(ushort*)pValue = analogue.wSendFlag;break;
//    case ATTR_ANA_EQUIPID:*(int*)pValue = analogue.btStationID;break;
    case ATTR_ANA_GROUPID:*(ushort*)pValue = analogue.wGroupID;break;
    case ATTR_ANA_POWERGRADE:*(int*)pValue = analogue.nPowerGrade;break;
    case ATTR_ANA_RELDIGITALID:*(ushort*)pValue = analogue.wRelDigitalID;break;
    case ATTR_ANA_VALUE :*(float*)pValue = analogue.fValue;break;
    default:
        return false;
    }
    return true;
}

bool HKerAnalogue::setAttr(quint16 wAttrib, void *pValue, HKerStation *pStation)
{
    return doSetAttr(wAttrib,pValue,pStation);
}

bool HKerAnalogue::doSetAttr(quint16 wAttrib, void *pValue, HKerStation *pStation)
{
    if(!pValue)
        return false;
    switch(wAttrib)
    {
    case ATTR_ANA_SNO :            analogue.wStationID = *(ushort*)pValue;break;
    case ATTR_ANA_NO  :            analogue.wAnalogueID = *(ushort*)pValue ;break;
    case ATTR_ANA_NAME :
    {
        qstrncpy(analogue.szAnalogueName,(char*)pValue,ANALOGUENAMELEN-1);
        analogue.szAnalogueName[ANALOGUENAMELEN - 1] = 0;
        break;
    }
    case ATTR_ANA_ORINAME:
    {
        qstrncpy(analogue.szAnalogueOriginalName,(char*)pValue,ANALOGUENAMELEN-1);
        analogue.szAnalogueOriginalName[ANALOGUENAMELEN - 1] = 0;
        break;
    }
    case ATTR_ANA_TYPE:             analogue.btAnalogueType = *(uchar*)pValue;break;
    case ATTR_ANA_UINT:             analogue.btUint = *(uchar*)pValue;break;
    case ATTR_ANA_CC1:              analogue.fCC1 = *(float*)pValue;break;
    case ATTR_ANA_CC2:              analogue.fCC2 = *(float*)pValue;break;
    case ATTR_ANA_CC3 :             analogue.fCC3 = *(float*)pValue;break;
    case ATTR_ANA_DIFF:             analogue.fDifference = *(float*)pValue;break;
    case ATTR_ANA_GRADE:            analogue.fGrades = *(float*)pValue;break;
    case ATTR_ANA_SENDFLAG:         analogue.wSendFlag = *(ushort*)pValue;break;
//    case ATTR_ANA_EQUIPID:*(int*)pValue = analogue.btStationID;break;
    case ATTR_ANA_GROUPID:          analogue.wGroupID = *(int*)pValue;break;
    case ATTR_ANA_POWERGRADE:       analogue.nPowerGrade = *(ushort*)pValue;break;
    case ATTR_ANA_RELDIGITALID:     analogue.wRelDigitalID = *(ushort*)pValue;break;
    case ATTR_ANA_VALUE :           analogue.fValue = *(float*)pValue;break;
    default:
        break;
    }

    return true;
}
