#include "SolCamera.hpp"

namespace SolEngine
{
    SolCamera::SolCamera(SolRenderer& rRenderer)
        : _rRenderer(rRenderer)
    {}

    SolCamera::SolCamera(SolRenderer& rRenderer, 
                         const PerspectiveProjectionInfo& projectionInfo)
        : _rRenderer(rRenderer),
          _projectionInfo(projectionInfo)
    {}

    void SolCamera::Update(const float deltaTime)
    {
        SetPerspectiveProjection(_projectionInfo);
    }

    SolCamera &SolCamera::SetPerspectiveProjection(const float fovDeg,
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

        return *this;
    }

    SolCamera& SolCamera::SetPerspectiveProjection(const PerspectiveProjectionInfo& projInfo)
    {
        return SetPerspectiveProjection(projInfo.fovDeg, 
                                        _rRenderer.GetAspectRatio(),
                                        projInfo.near, 
                                        projInfo.far);
    }

    SolCamera& SolCamera::LookAt(const glm::vec3& target, const glm::vec3& up)
    {
        _viewMatrix = glm::lookAtLH(_position, target, up);

        return *this;
    }

    SolCamera& SolCamera::SetProjectionInfo(const PerspectiveProjectionInfo& info)
    {
        _projectionInfo = info;

        SetPerspectiveProjection(_projectionInfo);

        return *this;
    }

    SolCamera& SolCamera::SetPosition(const glm::vec3& position)
    { 
        _position = position; 

        return *this;
    }
}