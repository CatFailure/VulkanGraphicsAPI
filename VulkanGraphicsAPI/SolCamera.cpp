#include "pch.hpp"
#include "SolCamera.hpp"

// minwindef has macros for near and far - for some reason...
// ... I need them, so disable it!
#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far

namespace SolEngine
{
    void SolCamera::SetOrthographicProjection(const float left, 
                                              const float right, 
                                              const float top, 
                                              const float bottom, 
                                              const float near, 
                                              const float far)
    {
        _projectionMatrix = glm::mat4{ 1.f };

        _projectionMatrix[0][0] = 2.f / (right - left);
        _projectionMatrix[1][1] = 2.f / (bottom - top);
        _projectionMatrix[2][2] = 1.f / (far - near);
        _projectionMatrix[3][0] = -(right + left) / (right - left);
        _projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        _projectionMatrix[3][2] = -near / (far - near);
    }

    void SolCamera::SetPerspectiveProjection(const float fov,
                                             const float aspect, 
                                             const float near,
                                             const float far)
    {
        assert(glm::abs(aspect - std::numeric_limits<float>::epsilon()) > 0.f);

        const float tanHalfFOV = tan(fov * .5f);
        float aspectNom = aspect;
        float aspectDen = 1.f;

        if (aspect < 1.f)
        {
            aspectNom = 1.f;
            aspectDen = 1.f / aspect;
        }

        _projectionMatrix = glm::mat4{ 0.f };

        _projectionMatrix[0][0] = 1.f / (aspectNom * tanHalfFOV);
        _projectionMatrix[1][1] = 1.f / (aspectDen * tanHalfFOV);
        _projectionMatrix[2][2] = far / (far - near);
        _projectionMatrix[2][3] = 1.f;
        _projectionMatrix[3][2] = -(far * near) / (far - near);
    }
}

#pragma pop_macro("near")
#pragma pop_macro("far")