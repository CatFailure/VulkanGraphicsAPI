#pragma once
#include "DebugHelpers.hpp"

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

        printf_s("%s - Disposed: %p.\n", __FUNCTION__, pArr);

        pArr = nullptr;
    }

    template<typename _Ty>
    static size_t AlignedMallocContiguousArray(_Ty*& prArr,
                                               const size_t size,
                                               const size_t align = 16U)
    {
        const size_t typeSizeBytes  = sizeof(_Ty);
        const size_t arraySizeBytes = size * align;

        prArr = (_Ty*)_aligned_malloc(arraySizeBytes, align);

        DBG_ASSERT_MSG((prArr != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, arraySizeBytes);

        return arraySizeBytes;
    }

    template<typename _Ty>
    static size_t AlignedMallocContiguous2DArray(_Ty *&prArr, 
                                                 const size_t rowCount, 
                                                 const size_t columnCount,
                                                 const size_t align = 16U)
    {
        // Must be indexed via my_matrix[row_index * row_length + column_index];
        const size_t typeSizeBytes  = sizeof(_Ty);
        const size_t arraySizeBytes = rowCount * columnCount * align;

        prArr = (_Ty *)_aligned_malloc(arraySizeBytes, align);

        DBG_ASSERT_MSG((prArr != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, arraySizeBytes);

        return arraySizeBytes;
    }
}