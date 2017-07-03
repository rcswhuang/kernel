#include "kerdigital.h"
#include "time.h"
#include "publicdata.h"
#include "kerstation.h"
HKerDigital::HKerDigital()
{

}

HKerDigital::~HKerDigital()
{

}
//标志结果等信息
bool HKerDigital::isEnable(int wEnable)//获取一些允许的属性 比如允许取反 允许报警等
{
    return (digital.wPFlag & wEnable);
}

bool HKerDigital::isResult(int wResult)//结果的属性 比如人工置数、确认、正常变位等
{
    return (digital.wRFlag & wResult);
}

void HKerDigital::enableFlag(int wFlag, bool bEnable, HKerStation *pStation)
{
    wFlag &= ENABLE_DIGITAL;
    if(bEnable && (wFlag == (wFlag & digital.wPFlag)) || !bEnable && (0 == (wFlag & digital.wPFlag)))
        return;
    if(bEnable)
    {
        digital.wPFlag |= wFlag;
    }
    else
    {
        digital.wPFlag &= ~wFlag;//这个还要确认一下
    }
}

void HKerDigital::doEnableFlag(int wFlag, bool bEnable, HKerStation *pStation)
{
    wFlag &= ENABLE_DIGITAL;
    if(bEnable && (wFlag == (wFlag & digital.wPFlag)) || !bEnable && (0 == (wFlag & digital.wPFlag)))
        return;

    HKerWord::enableFlag(wFlag,bEnable,pStation);
}

void HKerDigital::setResult(int wResult)//使具有一些结果属性
{
    digital.wRFlag |= wResult;
}

void HKerDigital::deSetResult(int wResult)//取消结果属性
{
    digital.wRFlag &= ~wResult;
}

bool HKerDigital::isOpen(quint8 btValue)
{
    return 0 == btValue;
}

bool HKerDigital::isClose(quint8 btValue)
{
    return 1 == btValue;
}

//双位置等信息
void HKerDigital::checkDS(HKerStation* pStation,long lTime)
{

}

void HKerDigital::checkValue(quint8 btValue,HKerStation* pStation,long lTime)
{
    deSetResult(RESULT_ACK);
    setResult(RESULT_CHANGE);
    //双位置遥信需要继续判断
    if(m_pTPDigital)
    {
        if(this == m_pTPDigital->pDigitalA)
        {
            m_pTPDigital->pDigitalB->digital.wRFlag &= ~RESULT_ACK;
            m_pTPDigital->pDigitalB->digital.wRFlag |= RESULT_CHANGE;
        }
        else if(this == m_pTPDigital->pDigitalB)
        {
            m_pTPDigital->pDigitalA->digital.wRFlag &= ~RESULT_ACK;
            m_pTPDigital->pDigitalA->digital.wRFlag |= RESULT_CHANGE;
        }
    }
}

void HKerDigital::doWrite(quint8 btValue,HKerStation* pStation,long lTime)
{
    //双位置判断
    if(m_pTPDigital)
    {
        m_pTPDigital->wWaitCount = 0;
        switch(m_pTPDigital->btState)
        {
        case TP_NORMAL:
        {
            m_pTPDigital->btOldValue = digital.btRunValue;
            if(m_pTPDigital->pDigitalA == this)
                m_pTPDigital->btState = TP_WAITB;
            else if(m_pTPDigital->pDigitalB == this)
                m_pTPDigital->btState = TP_WAITA;
            //break;
        }
            break;
        case TP_WAITA:
        {
            if(m_pTPDigital->pDigitalA == this)
                m_pTPDigital->btState = TP_NORMAL;
            else if(m_pTPDigital->pDigitalB == this)
                m_pTPDigital->btOldValue = digital.btRunValue;
            //break;
        }
            break;
        case TP_WAITB:
        {
            if(m_pTPDigital->pDigitalA == this)
                m_pTPDigital->btOldValue = digital.btRunValue;
            else if(m_pTPDigital->pDigitalB == this)
                m_pTPDigital->btState = TP_NORMAL;
        }
            break;
        }
    }


    digital.btRunValue = btValue;
    checkValue(digital.btRunValue,pStation,lTime);
}

//外部调此接口写入数据
bool HKerDigital::writeValue(quint8 btValue,HKerStation* pStation,long lTime)
{
    if(isResult(RESULT_MANUAL))
        return false;
    btValue = (quint8)(btValue & (quint8)0x03);
    if(isEnable(ENABLE_INE)) //取反
        btValue = (quint8)(btValue^(quint8)0x01);//异或
    writeValue(btValue,pStation,lTime);
    return true;
}

//确认 人工置数等
void HKerDigital::ack(HKerStation* pStation,long lTime)
{
    setResult(RESULT_ACK);
    //setAttr();
}

void HKerDigital::doManualSet(quint8 btValue,HKerStation* pStation)
{
    if(btValue > 3 || (isResult(RESULT_MANUAL) && btValue == digital.btRunValue ))
        return;
    setResult(RESULT_MANUAL);
    //双位置遥信 判断
    if(m_pTPDigital)
    {
        if(this == m_pTPDigital->pDigitalA)
        {
            m_pTPDigital->pDigitalB->digital.wRFlag |= RESULT_MANUAL;
        }
        else if(this == m_pTPDigital->pDigitalB)
        {
            m_pTPDigital->pDigitalA->digital.wRFlag |= RESULT_MANUAL;
        }
    }
    //写当前位置
    time_t lTime = time(NULL);
    if(btValue != digital.btRunValue)
    {
        doWrite(btValue,pStation,(long)lTime);
        if(m_pTPDigital)
        {
            uchar btInsValue;//
            if(this == m_pTPDigital->pDigitalA)
            {
                if(1 == btValue)
                    btInsValue = 0;
                else if(0 == btValue)
                    btInsValue = 1;
                else
                    btInsValue = btValue;
                m_pTPDigital->pDigitalB->digital.btRunValue = btInsValue;

                //通知B去发送变位遥信到其他模拟盘
            }
            else if(this == m_pTPDigital->pDigitalB)
            {
                if(1 == btValue)
                    btInsValue = 0;
                else if(0 == btValue)
                    btInsValue = 1;
                else
                    btInsValue = btValue;
                m_pTPDigital->pDigitalA->digital.btRunValue = btInsValue;
            }
            m_pTPDigital->btState = TP_NORMAL;
        }
    }
}

void HKerDigital::manualSet(quint8 btValue,HKerStation* pStation)
{
    if(btValue > 3 || (isResult(RESULT_MANUAL) && btValue == digital.btRunValue ))
        return;
    doManualSet(btValue,pStation);
}

void HKerDigital::doDeManualSet(HKerStation* pStation)
{
    deSetResult(RESULT_MANUAL);
    //如果是双位置遥信则同时取消
    if(m_pTPDigital)
    {
        if(this == m_pTPDigital->pDigitalA)
        {
            m_pTPDigital->pDigitalB->digital.wRFlag &= ~RESULT_MANUAL;
        }
        else if(this == m_pTPDigital->pDigitalB)
        {
            m_pTPDigital->pDigitalA->digital.wRFlag &= ~RESULT_MANUAL;
        }
    }
}

void HKerDigital::deManualSet(HKerStation* pStation)
{
    if(!isResult(RESULT_MANUAL))
        return;
    doDeManualSet(pStation);
}

//属性信息
quint16 HKerDigital::getIndex(HKerStation* pStation)
{
    return (this - pStation->pKerDigital);
}

quint16 HKerDigital::getNo()
{
    return digital.wDigitalID;
}

quint8  HKerDigital::getType()
{
    return TYPE_DIGITAL;
}

const char* HKerDigital::getName()
{
    return digital.szDigitalOriginalName;//需要增加判断是否是组合名称
}

quint16 HKerDigital::groupID()
{
    return 0;
}


bool HKerDigital::getAttr(quint16 wAttrib, void *pValue, HKerStation *pStation, quint32 size)
{
    if(!pValue)
        return false;
    switch(wAttrib)
    {
    case ATTR_DGT_SNO:          *(ushort*)pValue = digital.wStationID; break;
    case ATTR_DGT_NO:           *(ushort*)pValue = digital.wDigitalID;break;
    case ATTR_DGT_COMBONAME:
    {
        qstrcpy((char*)pValue,digital.szDigitalName);
        break;
    }
    case ATTR_DGT_ORINAME:
    {
        qstrcpy((char*)pValue,digital.szDigitalOriginalName);
        break;
    }
    case ATTR_DGT_POINTTERMID:  *(ushort*)pValue = digital.wPointTermID;break;
    case ATTR_DGT_EQUIPMENTID:
    {
        qstrcpy((char*)pValue, digital.szEquipmentID);//设备编号
    }
    case ATTR_DGT_GROUPID: *(ushort*)pValue = digital.wGroupID;break;
    case ATTR_DGT_POWERGRADE: *(ushort*)pValue = digital.nPowerGrade;break;
    case ATTR_DGT_GLOSSARYID: *(ushort*)pValue = digital.wGlossaryID;break;
    case ATTR_DGT_RULEFENID: *(ushort*)pValue = digital.wRuleFenID;break;
    case ATTR_DGT_RULEHEID: *(ushort*)pValue = digital.wRuleHeID;break;
    case ATTR_DGT_RULEJXFENID: *(ushort*)pValue = digital.wRuleJXFenID;break;
    case ATTR_DGT_RULEJXHEID: *(ushort*)pValue = digital.wRuleJXHeID;break;
    case ATTR_DGT_LOCKNO: *(ulong*)pValue = digital.dwLockNo;break;
    case ATTR_DGT_HELOCKNO: *(ulong*)pValue = digital.dwHeLockNo;break;
    case ATTR_DGT_FENLOCKNO: *(ulong*)pValue = digital.dwFenLockNo;break;
    case ATTR_DGT_SENDFLAG: *(ushort*)pValue = digital.wSendFlag;break;
    case ATTR_DGT_DOUBLEDGTID: *(ushort*)pValue = digital.wDoubleDgtID;break;
    case ATTR_DGT_OPFLAG: *(uchar*)pValue = digital.btOPFlag;break;
    case ATTR_DGT_FORMULAID: *(ushort*)pValue = digital.wFormulaID;//设备组
    case ATTR_DGT_VALUE: *(uchar*)pValue = digital.btRunValue;break;//还要加判计算遥信的情况
    case ATTR_DGT_RSNO: *(uchar*)pValue = digital.wRSNo;break;
    case ATTR_DGT_RNO: *(uchar*)pValue = digital.wRNo;break;
    case ATTR_DGT_MEASURE: *(uchar*)pValue = digital.wFormulaID;break;
    case ATTR_DGT_4_VALUE:
    {
        if(m_pTPDigital)
        {
            uchar btValue1,btValue2;
            switch(m_pTPDigital->btState)
            {
            case TP_NORMAL:
            {
                if(m_pTPDigital->pDigitalA == this)
                {
                    btValue1 = m_pTPDigital->pDigitalA->digital.btRunValue;
                    btValue2 = m_pTPDigital->pDigitalB->digital.btRunValue;
                }
                else
                {
                    btValue1 = m_pTPDigital->pDigitalB->digital.btRunValue;
                    btValue2 = m_pTPDigital->pDigitalA->digital.btRunValue;
                }
            }
                break;
            case TP_WAITA:
            {
                if(m_pTPDigital->pDigitalA == this)
                {
                    btValue1 = digital.btRunValue;
                    btValue2 = m_pTPDigital->btOldValue;
                }
                else
                {
                    btValue1 = m_pTPDigital->btOldValue;
                    btValue2 = m_pTPDigital->pDigitalA->digital.btRunValue;
                }
            }
                break;
            case TP_WAITB:
            {
                if(m_pTPDigital->pDigitalA == this)
                {
                    btValue1 = digital.btRunValue;
                    btValue2 = m_pTPDigital->btOldValue;
                }
                else
                {
                    btValue1 = m_pTPDigital->btOldValue;
                    btValue2 = m_pTPDigital->pDigitalB->digital.btRunValue;
                }
            }
                break;
            }
            *(uchar*)pValue = btValue1 + ((btValue1 == btValue2)?2:0);
        }
        else
            getAttr(wAttrib,pValue,pStation,0);
    }
        break;
    default:
        return false;
    }
    return true;
}

bool HKerDigital::setAttr(quint16 wAttrib, void *pValue, HKerStation *pStation)
{

    return doSetAttr(wAttrib,pValue,pStation);

}

bool HKerDigital::doSetAttr(quint16 wAttrib, void *pValue, HKerStation *pStation)
{
    if(!pValue)
        return false;
    switch(wAttrib)
    {
    case ATTR_DGT_SNO:           digital.wStationID = *(ushort*)pValue; break;
    case ATTR_DGT_NO:            digital.wDigitalID = *(ushort*)pValue ;break;
    case ATTR_DGT_COMBONAME:
    {
        qstrncpy(digital.szDigitalName,(char*)pValue,DIGITALNAMELEN-1);
        digital.szDigitalName[DIGITALNAMELEN - 1] = 0;
        break;
    }
    case ATTR_DGT_ORINAME:
    {
        qstrncpy(digital.szDigitalOriginalName,(char*)pValue,DIGITALNAMELEN -1);
        digital.szDigitalOriginalName[DIGITALNAMELEN - 1] = 0;
        break;
    }
    case ATTR_DGT_POINTTERMID:  digital.wPointTermID = *(ushort*)pValue ;break;
    case ATTR_DGT_EQUIPMENTID:
    {
        qstrncpy(digital.szEquipmentID,(char*)pValue, DIGITALNAMELEN -1);//设备编号
        digital.szEquipmentID[DIGITALNAMELEN - 1] = 0;
        break;
    }
    case ATTR_DGT_GROUPID:         digital.wGroupID = *(ushort*)pValue;break;
    case ATTR_DGT_POWERGRADE:      digital.nPowerGrade = *(ushort*)pValue;break;
    case ATTR_DGT_GLOSSARYID:      digital.wGlossaryID = *(ushort*)pValue;break;
    case ATTR_DGT_RULEFENID:       digital.wRuleFenID = *(ushort*)pValue;break;
    case ATTR_DGT_RULEHEID:        digital.wRuleHeID = *(ushort*)pValue;break;
    case ATTR_DGT_RULEJXFENID:     digital.wRuleJXFenID = *(ushort*)pValue;break;
    case ATTR_DGT_RULEJXHEID:      digital.wRuleJXHeID = *(ushort*)pValue;break;
    case ATTR_DGT_LOCKNO:          digital.dwLockNo = *(ulong*)pValue;break;
    case ATTR_DGT_HELOCKNO:        digital.dwHeLockNo = *(ulong*)pValue;break;
    case ATTR_DGT_FENLOCKNO:       digital.dwFenLockNo = *(ulong*)pValue;break;
    case ATTR_DGT_SENDFLAG:        digital.wSendFlag = *(ushort*)pValue;break;
    case ATTR_DGT_DOUBLEDGTID:     digital.wDoubleDgtID = *(ushort*)pValue;break;
    case ATTR_DGT_OPFLAG:          digital.btOPFlag = *(quint8*)pValue;break;
    case ATTR_DGT_FORMULAID:       digital.wFormulaID = *(ushort*)pValue;//设备组
    case ATTR_DGT_VALUE:           digital.btRunValue = *(quint8*)pValue;break;
    case ATTR_DGT_RSNO:            digital.wRSNo = *(ushort*)pValue;break;
    case ATTR_DGT_RNO:             digital.wRNo = *(ushort*)pValue;break;
    case ATTR_DGT_MEASURE:         digital.wFormulaID = *(quint8*)pValue;break;
    case ATTR_DGT_4_VALUE:
    {
        if(m_pTPDigital && m_pTPDigital->pDigitalA && m_pTPDigital->pDigitalB)
        {
            uchar btValue = *(uchar*)pValue;
            uchar btValue1,btValue2;
            if(btValue == 1)
            {
                btValue1 = 1;
                btValue1 = 0;
            }
            else if(btValue == 0)
            {
                btValue1 = 0;
                btValue2 = 1;
            }
            setAttr(ATTR_DGT_VALUE,pValue,pStation);
            if(this == m_pTPDigital->pDigitalA)
                m_pTPDigital->pDigitalB->setAttr(ATTR_DGT_VALUE,pValue,pStation);
            else if(this == m_pTPDigital->pDigitalB)
                m_pTPDigital->pDigitalA->setAttr(ATTR_DGT_VALUE,pValue,pStation);
        }
        else
            setAttr(ATTR_DGT_VALUE,pValue,pStation);
        break;
    }
    default:
        break;
    }
    return true;
}



