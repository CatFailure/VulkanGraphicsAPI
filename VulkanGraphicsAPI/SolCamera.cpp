#include "SolCamera.hpp"

namespace SolEngine
{
    SolCamera::SolCamera(SolRenderer& rRenderer, 
                         CameraSettings& rCameraSettings)
        : _rRenderer(rRenderer), 
          _rCameraSettings(rCameraSettings)
    {
        UpdatePerspectiveProjection();
    }

    void SolCamera::Update(const float deltaTime)
    {}

    SolCamera &SolCamera::UpdatePerspectiveProjection()
    {
        const float aspectRatio = _rRenderer.GetAspectRatio();

        DBG_ASSERT_MSG((glm::abs(aspectRatio - glm::epsilon<float>()) > 0.f),
                       "Aspect must be greater than 0!");

        const float tanHalfFOV = tan(glm::radians(_rCameraSettings.fieldOfViewDegrees) * .5f);

        float aspectNom = aspectRatio;
        float aspectDen = 1.f;

        // When the window has a smaller width than height,
        // This will scale the model to be smaller instead of staying the same size.
        if (aspectRatio < 1.f)
        {
            aspectNom = 1.f;
            aspectDen = 1.f / aspectRatio;
        }

        const float& near = _rCameraSettings.near;
        const float& far  = _rCameraSettings.far;

        _projectionMatrix = glm::mat4{ 0.f };

        _projectionMatrix[0][0] = 1.f / (aspectNom * tanHalfFOV);
        _projectionMatrix[1][1] = 1.f / (aspectDen * tanHalfFOV);
        _projectionMatrix[2][2] = far / (far - near);
        _projectionMatrix[2][3] = 1.f;
        _projectionMatrix[3][2] = -(far * near) / (far - near);

        return *this;
    }

    SolCamera& SolCamera::Move(const glm::vec3& delta)
    {
        _rCameraSettings.position += delta;

        return *this;
    }

    SolCamera& SolCamera::LookAt(const glm::vec3& target, const glm::vec3& up)
    {
        _viewMatrix = glm::lookAtLH(_rCameraSettings.position, target, up);

        return *this;
    }

    SolCamera& SolCamera::SetPosition(const glm::vec3& position)
    { 
        _rCameraSettings.position = position; 

        return *this;
    }
}