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
    static void FreeAlignedMallocArray(_Ty *pArr)
    {
        _aligned_free(pArr);
        pArr = nullptr;
    }

    template<typename _Ty>
    static void AlignedMallocContiguous2DArray(_Ty**& pprArr, 
                                               const size_t width, 
                                               const size_t height)
    {
        // https://stackoverflow.com/a/29375830
        const size_t sizeOfType = sizeof(_Ty);
        const size_t sizeOfPtrType = sizeof(_Ty *);
        const size_t alignment = 16U;

        pprArr    = (_Ty **)_aligned_malloc(width * sizeOfPtrType, alignment);

        DBG_ASSERT_MSG((pprArr != nullptr), "_aligned_malloc Failed!");

        pprArr[0] = (_Ty *)_aligned_malloc(width * height * sizeOfType, alignment);

        DBG_ASSERT_MSG((pprArr[0] != nullptr), "_aligned_malloc Failed!");

        for (size_t i(1); i < width; ++i)
        {
            pprArr[i] = pprArr[i - 1] + height;
        }
    }
}