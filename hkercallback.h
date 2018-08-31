#ifndef HKERCALLBACK_H
#define HKERCALLBACK_H
#include "publicdata.h"
/******************************************定义回调函数********************************/
#include "huserdbapi.h"
//插件回调函数
bool __cdecl pluginCallback(int nMsgType,HWPARAM wParam,HLPARAM lParam);

#endif // HKERCALLBACK_H
