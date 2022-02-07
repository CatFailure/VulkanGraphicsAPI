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

    template<typename _Ty>
    static void AllocateContiguous2DArray(_Ty**& pprArr, 
                                          const size_t width, 
                                          const size_t height)
    {
        // https://stackoverflow.com/a/29375830
        pprArr    = new _Ty *[width];
        pprArr[0] = new _Ty[width * height];

        for (size_t i(1); i < width; ++i)
        {
            pprArr[i] = pprArr[i - 1] + height;
        }
    }
}