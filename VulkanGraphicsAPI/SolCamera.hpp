#pragma once
#include "CameraProjectionInfo.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;
using namespace Utility;

namespace SolEngine
{
    class SolCamera : public IMonoBehaviour
    {
    public:
        // Inherited from IMonoBehaviour
        virtual void Update(const float deltaTime) override;

        /// <summary>
        /// Sets the Camera to use Orthographic Projection.
        /// </summary>
        /// <param name="left"> Left boundary plane. </param>
        /// <param name="right"> Right boundary plane. </param>
        /// <param name="top"> Top boundary plane. </param>
        /// <param name="bottom"> Bottom boundary plane. </param>
        /// <param name="near"> Near clipping distance. </param>
        /// <param name="far"> Far clipping distance. </param>
        void SetOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near = CAM_NEAR_DEFAULT, const float far = CAM_FAR_DEFAULT);
        void SetOrthographicProjection(const OrthographicProjectionInfo &projInfo);

        /// <summary>
        /// Sets the Camera to use Perspective Projection.
        /// </summary>
        /// <param name="fovDeg"> Vertical Field-of-view in Degrees. </param>
        /// <param name="aspect"> Aspect Ratio of window. </param>
        /// <param name="near"> Near clipping distance. </param>
        /// <param name="far"> Far clipping distance. </param>
        void SetPerspectiveProjection(const float fovDeg, const float aspect, const float near = CAM_NEAR_DEFAULT, const float far = CAM_FAR_DEFAULT);
        void SetPerspectiveProjection(const PerspectiveProjectionInfo &projInfo);

        glm::vec3 GetPosition()             const { return _position; }
        glm::vec3 GetRight()                const;
        glm::vec3 GetUp()                   const;
        glm::vec3 GetForward()              const;
        glm::mat4 GetProjectionMatrix()     const { return _projectionMatrix; }
        glm::mat4 GetViewMatrix()           const { return _viewMatrix; }
        glm::mat4 GetProjectionViewMatrix() const { return _projectionMatrix * _viewMatrix; }

        void SetPosition(const glm::vec3& position) { _position = position; }

    private:
        void UpdateViewMatrix();

        bool _isViewMatrixDirty{ true };

        glm::vec3 _position{ 0, 0, 0 };

        float _yaw{ 0.f };
        float _pitch{ 0.f };
        float _roll{ 0.f };

        glm::mat4 _projectionMatrix{ 1.f };
        glm::mat4 _viewMatrix      { 1.f };
    };
}
