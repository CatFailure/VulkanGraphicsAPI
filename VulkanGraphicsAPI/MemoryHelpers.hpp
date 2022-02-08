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
        const size_t sizeOfType    = sizeof(_Ty);
        const size_t sizeOfPtrType = sizeof(_Ty *);
        const size_t arraySizeBytes = width * sizeOfPtrType;
        const size_t arrayElementsSizeBytes = width * height * sizeOfType;
        const size_t alignment     = 16U;

        pprArr = (_Ty **)_aligned_malloc(arraySizeBytes, alignment);

        DBG_ASSERT_MSG((pprArr != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, arraySizeBytes);

        pprArr[0] = (_Ty *)_aligned_malloc(arrayElementsSizeBytes, alignment);

        DBG_ASSERT_MSG((pprArr[0] != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, arrayElementsSizeBytes);

        for (size_t i(1); i < width; ++i)
        {
            pprArr[i] = pprArr[i - 1] + height;
        }
    }

    template<typename _Ty>
    static void AlignedMallocContiguous2DArray2(_Ty *&prArr, 
                                                const size_t rowCount, 
                                                const size_t columnCount)
    {
        // Must be indexed via my_matrix[row_index * row_length + column_index];
        const size_t sizeOfType = sizeof(_Ty);
        const size_t sizeOfArrayBytes = rowCount * columnCount * sizeOfType;
        const size_t alignment = 16U;

        prArr = (_Ty *)_aligned_malloc(sizeOfArrayBytes, alignment);

        DBG_ASSERT_MSG((prArr != nullptr), "_aligned_malloc Failed!");
        printf_s("%s - Allocated: %zu bytes.\n", __FUNCTION__, sizeOfArrayBytes);
    }
}