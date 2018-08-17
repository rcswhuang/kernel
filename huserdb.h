#ifndef HUSERDB_H
#define HUSERDB_H
#include <QObject>
#include <QLibrary>
#include "hkernelapi.h"

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
    bool initProc();//初始化插件
    bool exitProc(); //退出插件
    void updateData(void* handle,UPDATAPOINT* point);//发送数据
    void showMsgWindow(bool bShow);//显示窗口
    void showConfigWindow();
public:
    QString strUserDBName;
    PLUGINPROC pluginProc;//为所有插件统一处理函数地址，回调函数
};

#endif // HUSERDB_H
