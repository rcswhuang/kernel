#include "hkercallback.h"
#include "hkernelhandle.h"
#include "huserdbapi.h"
bool __cdecl pluginCallback(int nMsgType,HWPARAM wParam,HLPARAM lParam)
{
    switch (nMsgType) {
    case PM_GETDBINFO:
    {
        PLUGINPARAM *param = (PLUGINPARAM*)lParam;
        if(TYPE_DIGITAL == param->btType)
        {
            DBHANDLE dbHandle = HKernelHandle::Instance()->getDbHandle(param->wStation,param->btType,param->wPoint,0);
            if(!HKernelHandle::Instance()->isValidDbHandle(dbHandle))
                return false ;
            uchar btValue;
            bool bok = HKernelHandle::Instance()->getAttr(dbHandle,param->wAttrib,&btValue);
            if(!bok) return false;
            param->pBuffer = &btValue;
            return true;
        }

        if(TYPE_ANALOGUE == param->btType)
        {
            DBHANDLE dbHandle = HKernelHandle::Instance()->getDbHandle(param->wStation,param->btType,param->wPoint,0);//必须是实时库
            if(!HKernelHandle::Instance()->isValidDbHandle(dbHandle))
                return false ;
            float fValue;
            bool bok = HKernelHandle::Instance()->getAttr(dbHandle,param->wAttrib,&fValue);
            if(!bok) return false;
            param->pBuffer = &fValue;
            return true;
        }
    }
    default:
            return false;
    }
    return true;
}
