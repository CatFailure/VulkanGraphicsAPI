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

    void SolCamera::SetPerspectiveProjection(const float fovDeg,
                                             const float aspect, 
                                             const float near,
                                             const float far)
    {
        DBG_ASSERT_MSG((glm::abs(aspect - glm::epsilon<float>()) > 0.f),
                       "Aspect must be greater than 0!");

        const float tanHalfFOV = tan(glm::radians(fovDeg) * .5f);
        float aspectNom = aspect;
        float aspectDen = 1.f;

        // When the window has a smaller width than height,
        // This will scale the cube to be smaller instead of staying the same size.
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

    void SolCamera::SetViewDirection(const glm::vec3 &position, 
                                     const glm::vec3 &direction, 
                                     const glm::vec3 &up)
    {
        // Construct an Orthonormal Basis -
        // 3 Vectors, all unit length and orthogonal (at 90deg angles to eachother)
        const glm::vec3 w{ glm::normalize(direction) };
        const glm::vec3 u{ glm::normalize(glm::cross(w, up)) };
        const glm::vec3 v{ glm::cross(w, u) };

        _viewMatrix = glm::mat4{ 1.f };

        _viewMatrix[0][0] = u.x;
        _viewMatrix[1][0] = u.y;
        _viewMatrix[2][0] = u.z;

        _viewMatrix[0][1] = v.x;
        _viewMatrix[1][1] = v.y;
        _viewMatrix[2][1] = v.z;

        _viewMatrix[0][2] = w.x;
        _viewMatrix[1][2] = w.y;
        _viewMatrix[2][2] = w.z;

        _viewMatrix[3][0] = -glm::dot(u, position);
        _viewMatrix[3][1] = -glm::dot(v, position);
        _viewMatrix[3][2] = -glm::dot(w, position);
    }

    void SolCamera::LookAt(const glm::vec3 &position, 
                           const glm::vec3 &target, 
                           const glm::vec3 &up)
    {
        const glm::vec3 direction = target - position;  // B - A

        // Prevent non-zero directions.
        DBG_ASSERT_MSG((glm::dot(direction, direction) > glm::epsilon<float>()), 
                       "Direction must be non-zero!");

        SetViewDirection(position, 
                         direction,
                         up);
    }

    void SolCamera::SetView(const glm::vec3 &position, 
                            const glm::vec3 &rotation)
    {
        const glm::vec3 rotRad
        {
            glm::radians(-rotation.x),
            glm::radians(rotation.y),
            glm::radians(rotation.z),
        };

        // Construct inverse rotation matrix.
        // Combine with translation back to origin for view matrix.
        const float cosRotX = glm::cos(rotRad.x);
        const float sinRotX = glm::sin(rotRad.x);
        const float cosRotY = glm::cos(rotRad.y);
        const float sinRotY = glm::sin(rotRad.y);
        const float cosRotZ = glm::cos(rotRad.z);
        const float sinRotZ = glm::sin(rotRad.z);

        const glm::vec3 u
        { 
            cosRotY * cosRotZ + sinRotY * sinRotX * sinRotZ, 
            cosRotX * sinRotZ, 
            cosRotY * sinRotX * sinRotZ - cosRotZ * sinRotY 
        };

        const glm::vec3 v
        {
            cosRotZ * sinRotY * sinRotX - cosRotY * sinRotZ, 
            cosRotX * cosRotZ, 
            cosRotY * cosRotZ * sinRotX + sinRotY * sinRotZ 
        };

        const glm::vec3 w
        { 
            cosRotX * sinRotY, 
            -sinRotX,
            cosRotY * cosRotX 
        };

        _viewMatrix = glm::mat4{ 1.f };

        _viewMatrix[0][0] = u.x;
        _viewMatrix[1][0] = u.y;
        _viewMatrix[2][0] = u.z;

        _viewMatrix[0][1] = v.x;
        _viewMatrix[1][1] = v.y;
        _viewMatrix[2][1] = v.z;

        _viewMatrix[0][2] = w.x;
        _viewMatrix[1][2] = w.y;
        _viewMatrix[2][2] = w.z;

        _viewMatrix[3][0] = -glm::dot(u, position);
        _viewMatrix[3][1] = -glm::dot(v, position);
        _viewMatrix[3][2] = -glm::dot(w, position);
    }
}

#pragma pop_macro("near")
#pragma pop_macro("far")