#include "userdb.h"
#include <QLibrary>
HUserDb::HUserDb()
{
    pluginProc = NULL;
}

HUserDb::~HUserDb()
{
  //不需要手动释放lib,lib在程序终止后自动释放。
}

bool HUserDb::funcName(char* szFuncName)
{
    return pluginProc(PLG_PROCNAME,0,(ulong)szFuncName);
}

bool HUserDb::loadData()
{
    return pluginProc(PLG_LOADDATA,0,0);
}

bool HUserDb::saveData()
{
    return pluginProc(PLG_SAVEDATA,0,0);
}

bool HUserDb::initProc()
{
    return pluginProc(PLG_INITPROC,0,0);
}

bool HUserDb::exitProc()
{
    return pluginProc(PLG_EXITPROC,0,0);
}

void HUserDb::updateData(void* handle,UPDATAPOINT* point)
{
    pluginProc(PLG_CHANGEDYX,(unsigned int)handle,(long)point);
}

void HUserDb::showMsgWindow(bool bShow)
{
    pluginProc(PLG_SHOWMSGWIN,0,(ulong)bShow);
}

void HUserDb::showConfigWindow()
{
    pluginProc(PLG_SHOWCONFIGWIN,0,0);
}
