#pragma once
#include "Vector.hpp"

namespace SolEngine::Math
{
    template<typename _Ty>
    struct Rect
    {
        Rect()
        {}

        Rect(const _Ty left_, 
             const _Ty top_, 
             const _Ty width_, 
             const _Ty height_)
            : left(left_),
              top(top_), 
              width(width_), 
              height(height_)
        {}

        Rect(const Vector2<_Ty> &position, 
             const Vector2<_Ty> &size)
            : left(position.x), 
              top(position.y), 
              width(size.x), 
              height(size.y)
        {}

        _Ty left, top, width, height;
    };

    typedef Rect<int>      IntRect;
    typedef Rect<uint32_t> UIntRect;
    typedef Rect<float>    FloatRect;
    typedef Rect<double>   DoubleRect;
}
