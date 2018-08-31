#include "huserdb.h"
#include <QLibrary>
HUserDb::HUserDb()
{
    m_pluginProc = NULL;
}

HUserDb::~HUserDb()
{
  //不需要手动释放lib,lib在程序终止后自动释放。
}

bool HUserDb::pluginInfo()
{
    quint16 pluginID = 0;
    char pluginName[128];
    bool bok = m_pluginProc(PLG_PROCNAME,pluginID,(ulong)pluginName);
    m_npluginID = pluginID;
    m_strPluginName = pluginName;
    return bok;
}

bool HUserDb::loadData()
{
    return m_pluginProc(PLG_LOADDATA,0,0);
}

bool HUserDb::saveData()
{
    return m_pluginProc(PLG_SAVEDATA,0,0);
}

bool HUserDb::initProc()
{
    return m_pluginProc(PLG_INITPROC,0,0);
}

void HUserDb::pluginConfig(LPPLUGINPROC pluginCallback)
{
    m_pluginProc(PLG_PLUGINCONFIG,0,(long long)pluginCallback);
}

bool HUserDb::exitProc()
{
    return m_pluginProc(PLG_EXITPROC,0,0);
}

void HUserDb::updateData(void* handle,UPDATAPOINT* point)
{
    //m_pluginProc(PLG_CHANGEDYX,(unsigned int)handle,(long)point);
}

void HUserDb::showMsgWindow(bool bShow)
{
    m_pluginProc(PLG_SHOWMSGWIN,0,(ulong)bShow);
}

void HUserDb::showConfigWindow()
{
    m_pluginProc(PLG_SHOWCONFIGWIN,0,0);
}
