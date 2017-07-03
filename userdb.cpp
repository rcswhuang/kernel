#include "userdb.h"
#include <QLibrary>
HUserDb::HUserDb()
{
    userdbProc = NULL;
}

HUserDb::~HUserDb()
{
  //不需要手动释放lib,lib在程序终止后自动释放。
}

bool HUserDb::funcName(char* szFuncName)
{
    return userdbProc(UDB_PLUGINNAME,0,(ulong)szFuncName);
}

bool HUserDb::loadData()
{
    return userdbProc(UDB_LOADDATA,0,0);
}

bool HUserDb::saveData()
{
    return userdbProc(UDB_SAVEDATA,0,0);
}

bool HUserDb::initLibrary()
{
    return userdbProc(UDB_INITLIBRARY,0,0);
}

bool HUserDb::exitLibrary()
{
    return userdbProc(UDB_EXITLIBRARY,0,0);
}

void HUserDb::updateData(UDBHANDLE udbHandle,UDBDATAPARAM* udbParam)
{
    userdbProc(UDB_UPDATEDATA,(ulong)udbHandle,(ulong)udbParam);
}

void HUserDb::showWindow(bool bShow)
{
    userdbProc(UDB_SHOWWINDOW,0,(ulong)bShow);
}
