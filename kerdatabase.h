#ifndef HKERDATABASE_H
#define HKERDATABASE_H

#include <QObject>
#include <QTimer>
#include <QList>
#include "kerstation.h"
#include "kerword.h"
#include "pointterm.h"
#include "userdb.h"
#include "glossary.h"
#include "locktype.h"
#include "kerdigital.h"
#include "keranalogue.h"
class HKerStation;//厂站
class HLockType;
class HPointTerm;//测点类型
class HGlossaryGroup;//操作术语
class HUserDb;

class HKerDataBase : public QObject
{
    Q_OBJECT
public:
    explicit HKerDataBase(QObject *parent = 0);
    ~HKerDataBase();

public:

    //数据相关操作
    bool loadData();
    void saveData();
    void freeData();
    bool loadAnalogue();
    void saveAnalogue();//保存遥测的实时值
    bool loadDigital();
    void saveDigital();//保存遥信的实时值

    //实时数据库厂站操作
    quint16 getStationNum();
    quint16 getStationIndex(HKerStation* pKerStation);
    HKerStation* getKerRtu(quint16 wAddr);
    HKerStation* getKerStation(quint16 wNo);
    HKerStation* findKerStation(quint16 wIndex);


    //厂站内的遥信遥测数据的读取
    //获取厂站内的遥信遥测类型的第wNo测点信息
    HKerWord* getKerWord(quint16 wStation,quint8 btType,quint16 wNo);
    HKerWord* findKerWord(quint16 wStation,quint8 btType,quint16 wIndex);

    //遥控部分 五防暂时不实现 一般五防不会直接遥控
    //锁类型 依赖厂站
    HLockType* findLockType(ushort wStation,QString strLockType);
    HLockType* findLockType(ushort wStation,ushort wLockTypeID);

    //测点类型操作 不依赖厂站
    HPointTerm* findPointTerm(ushort wPointTermID);
    HPointTerm* findPointTerm(QString strPointTerm);

    //操作术语操作 不依赖厂站
    ushort getGlossaryGroupNum();
    ushort getGlossaryNum();
    HGlossaryGroup* getGlossaryGroup(ushort wNo);
    GLOSSARY* getGlossary(ushort wGlossaryGroupID,uchar btType,ushort wGlossaryID);
    //插件操作


public:
    ushort wTotalStation;
    ushort wTotalPointTerm;
    ushort wTotalLockType;
    ushort wTotalGlossaryGroup;
    ushort wTotalGlossary;
    ushort wTotalUserDb;
    int nDataBaseID;
    QTimer *dsTimer; //定时判断双位置遥信的状态
public:
    HKerStation* pKerStation;//厂站
    HLockType* pLockType;
    HPointTerm*  pPointTerm;//测点类型
    HGlossaryGroup* pGlossaryGroup;//操作术语
    HUserDb* pUserDb; //插件代理
signals:

public slots:
    void dsDigitalUpdate();//双位置遥信定时刷新
};

#endif // HKERDATABASE_H
