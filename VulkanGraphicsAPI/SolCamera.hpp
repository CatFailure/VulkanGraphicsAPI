#pragma once
#define GLM_FORCE_RADIANS	// Make glm functions expect angles in radians instead of degrees

#include "SolRenderer.hpp"
#include "IMonoBehaviour.hpp"
#include "Constants.hpp"
#include "CameraSettings.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;
using namespace SolEngine::Settings;
using namespace Utility;

namespace SolEngine
{
    class SolCamera : public IMonoBehaviour
    {
    public:
        SolCamera() = delete;
        SolCamera(SolRenderer& rRenderer, CameraSettings& rCameraSettings);

        // Inherited from IMonoBehaviour
        virtual void Update(const float deltaTime) override;

        /// <summary>
        /// Updates the Camera's Perspective Projection Matrix.
        /// </summary>
        SolCamera& UpdatePerspectiveProjection();

        SolCamera& Move(const glm::vec3& delta);
        SolCamera& LookAt(const glm::vec3& target, const glm::vec3& up = VEC3_UP);

        const glm::vec3& GetPosition()             const { return _rCameraSettings.position; }
        const glm::mat4& GetProjectionMatrix()     const { return _projectionMatrix; }
        const glm::mat4& GetViewMatrix()           const { return _viewMatrix; }
        glm::mat4        GetProjectionViewMatrix() const { return _projectionMatrix * _viewMatrix; }

        SolCamera& SetPosition(const glm::vec3& position);

    private:
        SolRenderer&    _rRenderer;
        CameraSettings& _rCameraSettings;

        glm::mat4 _projectionMatrix{ 1.f };
        glm::mat4 _viewMatrix      { 1.f };
    };
}
