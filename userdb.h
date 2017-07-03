#ifndef HUSERDB_H
#define HUSERDB_H
#include <QObject>
#include <QLibrary>
#include "kernelapi.h"

class HUserDb:public QObject
{
    Q_OBJECT
public:
    HUserDb();
    ~HUserDb();

public:
    bool funcName(char* szFuncName);//获取插件名称
    bool loadData();//加载数据
    bool saveData();//保存数据
    bool initLibrary();//初始化插件
    bool exitLibrary(); //退出插件
    void updateData(UDBHANDLE udbHandle,UDBDATAPARAM* udbParam);//发送数据
    void showWindow(bool bShow);//显示窗口
public:
    QString strUserDBName;
    USERDBPROC userdbProc;
};

#endif // HUSERDB_H
