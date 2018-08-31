#ifndef HUSERDB_H
#define HUSERDB_H
#include <QObject>
#include <QLibrary>
#include "hkernelapi.h"
#include "huserdbapi.h"
class HUserDb:public QObject
{
    Q_OBJECT
public:
    HUserDb();
    ~HUserDb();
public:
    bool pluginInfo();//获取插件名称
    bool loadData();//加载数据
    bool saveData();//保存数据
    bool initProc();//初始化插件
    void pluginConfig(LPPLUGINPROC pluginCallback);//插件配置
    bool exitProc(); //退出插件
    void updateData(void* handle,UPDATAPOINT* point);//发送数据
    void showMsgWindow(bool bShow);//显示窗口
    void showConfigWindow();
public:
    QString m_strPluginName;
    quint16 m_npluginID;
    //数据库组态、运行实时库都需要从插件里面获取相关内容
    PLUGINPROC m_pluginProc;//为所有插件统一处理函数地址，回调函数

};

#endif // HUSERDB_H
