#include "pch.hpp"
#include "SolCamera.hpp"

namespace SolEngine
{
    void SolCamera::Update(const float deltaTime)
    {
        UpdateViewMatrix();

        //if (_isViewMatrixDirty)
        //{
        //    UpdateViewMatrix();
        //}
    }

    void SolCamera::SetOrthographicProjection(const float left, 
                                              const float right,
                                              const float top, 
                                              const float bottom,
                                              const float near, 
                                              const float far)
    {
        //_projectionMatrix = glm::mat4{ 1.f };

        _projectionMatrix = glm::orthoLH(left, right, bottom, top, near, far);

        //_projectionMatrix[0][0] = 2.f / (right - left);
        //_projectionMatrix[1][1] = 2.f / (bottom - top);
        //_projectionMatrix[2][2] = 1.f / (far - near);
        //_projectionMatrix[3][0] = -(right + left) / (right - left);
        //_projectionMatrix[3][1] = -(bottom + top) / (bottom - top);
        //_projectionMatrix[3][2] = -near / (far - near);
    }

    void SolCamera::SetOrthographicProjection(const OrthographicProjectionInfo& projInfo)
    {
        SetOrthographicProjection(projInfo.left, projInfo.right, projInfo.top, projInfo.bottom, projInfo.near, projInfo.far);
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

    void SolCamera::SetPerspectiveProjection(const PerspectiveProjectionInfo &projInfo)
    {
        SetPerspectiveProjection(projInfo.fovDeg, projInfo.aspect, projInfo.near, projInfo.far);
    }

    glm::vec3 SolCamera::GetRight() const
    {
        return glm::vec3(_viewMatrix[0].x, _viewMatrix[1].x, _viewMatrix[2].x);
    }

    glm::vec3 SolCamera::GetUp() const
    {
        return glm::vec3(_viewMatrix[0].y, _viewMatrix[1].y, _viewMatrix[2].y);
    }

    glm::vec3 SolCamera::GetForward() const
    {
        return glm::vec3(_viewMatrix[0].z, _viewMatrix[1].z, _viewMatrix[2].z);
    }
    
    void SolCamera::UpdateViewMatrix()
    {
        const float pitchCos(cosf(_pitch)), pitchSin(sinf(_pitch));
        const float yawCos(cosf(_yaw)), yawSin(sinf(_yaw));

        const glm::vec3 axisX{ yawCos, 0, -yawSin };
        const glm::vec3 axisY{ yawSin * pitchSin, pitchCos, yawCos * pitchSin };
        const glm::vec3 axisZ{ yawSin * pitchCos, -pitchSin, pitchCos * yawCos };

        _viewMatrix =
        {
            { axisX.x, axisY.x, axisZ.x, 0 },
            { axisX.y, axisY.y, axisZ.y, 0 },
            { axisX.z, axisY.z, axisZ.z, 0 },
            { -dot(axisX, _position), -dot(axisY, _position), -dot(axisZ, _position), 1 },
        };
    }
}