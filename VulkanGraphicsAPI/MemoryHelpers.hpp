#pragma once

namespace Utility
{
    template<typename _Ty>
    static void SafeDispose(_Ty *p)
    {
        delete p;
        p = nullptr;
    }

    template<typename _Ty>
    static void SafeDisposeArray(_Ty *pArr)
    {
        delete[] pArr;
        pArr = nullptr;
    }
}