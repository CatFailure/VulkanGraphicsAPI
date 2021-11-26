#pragma once

namespace SolEngine::Math
{
    template<class _Ty>
    class VectorAbstract
    {
    public:
        virtual void Normalise() = 0;
        virtual _Ty Normalised() const = 0;
    };

    template<class _Ty>
    class Vector2 : public VectorAbstract<_Ty>
    {
    public:
        Vector2()
        {}

        Vector2(const _Ty x, const _Ty y)
            : _x(x), _y(y)
        {}

        Vector2(const Vector2<_Ty> &vec2)
            :_x(vec2._x), _y(vec2._y)
        {}

        static Vector2<_Ty> Zero() { return Vector2<_Ty>(); }
        static Vector2<_Ty> One() { return Vector2<_Ty>(1, 1); }

        void Normalise() override
        {
            _Ty vecLength = Normalised();

            if (vecLength == 0)
            {
                return;
            }

            _x /= vecLength;
            _y /= vecLength;
        }

        _Ty Normalised() const override
        {
            return static_cast<_Ty>(std::sqrt((_x * _x) + (_y * _y)));
        }

        _Ty _x{ 0 };
        _Ty _y{ 0 };
    };

    template<class _Ty>
    class Vector3 : VectorAbstract<_Ty>
    {
    public:
        Vector3()
        {}

        Vector3(const _Ty x, const _Ty y, const _Ty z)
            : _x(x), _y(y), _z(z)
        {}

        Vector3(const _Ty x, const _Ty y)
            : Vector3(x, y, 0)
        {}

        Vector3(const Vector2<_Ty> &vec2, const _Ty z)
            : _x(vec2._x), _y(vec2._y), _z(z)
        {}

        Vector3(const Vector2<_Ty> &vec2)
            : Vector3(vec2, 0)
        {}

        static Vector3<_Ty> Zero() { return Vector3<_Ty>(); }
        static Vector3<_Ty> One() { return Vector3<_Ty>(1, 1, 1); }

        void Normalise() override
        {
            _Ty vecLength = Normalised();

            if (vecLength == 0)
            {
                return;
            }

            _x /= vecLength;
            _y /= vecLength;
            _z /= vecLength;
        }

        _Ty Normalised() const override
        {
            return static_cast<_Ty>(std::sqrt((_x * _x) + (_y * _y) + (_z * _z)));
        }

        _Ty _x{ 0 };
        _Ty _y{ 0 };
        _Ty _z{ 0 };
    };
}

