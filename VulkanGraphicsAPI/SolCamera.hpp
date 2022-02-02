#pragma once
#include "SolRenderer.hpp"
#include "CameraProjectionInfo.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;
using namespace Utility;

namespace SolEngine
{
    class SolCamera : public IMonoBehaviour
    {
    public:
        SolCamera(SolRenderer &rRenderer);
        SolCamera(SolRenderer &rRenderer, const PerspectiveProjectionInfo &projectionInfo);

        // Inherited from IMonoBehaviour
        virtual void Update(const float deltaTime) override;

        /// <summary>
        /// Sets the Camera to use Perspective Projection.
        /// </summary>
        /// <param name="fovDeg"> Vertical Field-of-view in Degrees. </param>
        /// <param name="aspect"> Aspect Ratio of window. </param>
        /// <param name="near"> Near clipping distance. </param>
        /// <param name="far"> Far clipping distance. </param>
        void SetPerspectiveProjection(const float fovDeg, const float aspect, const float near = CAM_NEAR_DEFAULT, const float far = CAM_FAR_DEFAULT);
        void SetPerspectiveProjection(const PerspectiveProjectionInfo &projInfo);

        void LookAt(const glm::vec3 &target, const glm::vec3 &up = VEC3_UP);

        glm::vec3 GetPosition()             const { return _position; }
        glm::mat4 GetProjectionMatrix()     const { return _projectionMatrix; }
        glm::mat4 GetViewMatrix()           const { return _viewMatrix; }
        glm::mat4 GetProjectionViewMatrix() const { return _projectionMatrix * _viewMatrix; }

        void SetProjectionInfo(const PerspectiveProjectionInfo &info);
        void SetPosition(const glm::vec3& position) { _position = position; }

    private:
        SolRenderer &_rRenderer;
        PerspectiveProjectionInfo _projectionInfo{};

        glm::vec3 _position{ 0, 0, 0 };

        glm::mat4 _projectionMatrix{ 1.f };
        glm::mat4 _viewMatrix      { 1.f };
    };
}
