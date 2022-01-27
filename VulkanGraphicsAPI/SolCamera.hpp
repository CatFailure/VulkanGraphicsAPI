#pragma once
#include "CameraProjectionInfo.hpp"

using namespace SolEngine::Data;
using namespace SolEngine::Interface;
using namespace Utility;

namespace SolEngine
{
    //class SolCamera
    //{
    //public:
    //    /// <summary>
    //    /// Sets the Camera to use Orthographic Projection.
    //    /// </summary>
    //    /// <param name="left"> Left boundary plane. </param>
    //    /// <param name="right"> Right boundary plane. </param>
    //    /// <param name="top"> Top boundary plane. </param>
    //    /// <param name="bottom">Bottom boundary plane. </param>
    //    /// <param name="near"> Near clipping distance. </param>
    //    /// <param name="far"> Far clipping distance. </param>
    //    void SetOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near, const float far);

    //    /// <summary>
    //    /// Sets the Camera to use Perspective Projection.
    //    /// </summary>
    //    /// <param name="fovDeg"> Vertical Field-of-view in Degrees. </param>
    //    /// <param name="aspect"> Aspect Ratio of window. </param>
    //    /// <param name="near"> Near clipping distance. </param>
    //    /// <param name="far"> Far clipping distance. </param>
    //    void SetPerspectiveProjection(const float fovDeg, const float aspect, const float near, const float far);

    //    /// <summary>
    //    /// Points camera in a desired direction.
    //    /// </summary>
    //    void SetViewDirection(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up = glm::vec3(0.f, -1.f, 0.f));

    //    /// <summary>
    //    /// Points camera towards a specific point.
    //    /// </summary>
    //    void LookAt(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up = glm::vec3(0.f, -1.f, 0.f));

    //    /// <summary>
    //    /// Sets camera transform - Rotation follows Tait-Bryan Y(1), X(2), Z(3)
    //    /// </summary>
    //    void SetView(const glm::vec3& position, const glm::vec3& rotation);

    //    glm::mat4 GetProjection()     const { return _projectionMatrix; }
    //    glm::mat4 GetView()		      const { return _viewMatrix; }
    //    glm::mat4 GetProjectionView() const { return _projectionMatrix * _viewMatrix; }

    //private:
    //    glm::mat4 _projectionMatrix{ 1.f };
    //    glm::mat4 _viewMatrix{ 1.f };	// Stores the Camera's transform
    //};

    class SolCamera : public IMonoBehaviour
    {
    public:
        // Inherited from IMonoBehaviour
        virtual void Update(const float deltaTime) override;

        glm::vec3 GetPosition()             const { return _position; }
        glm::vec3 GetRotation()             const { return _rotation; }
        glm::mat4 GetProjectionMatrix()     const { return _projectionMatrix; }
        glm::mat4 GetViewMatrix()           const { return _viewMatrix; }
        glm::mat4 GetProjectionViewMatrix() const { return _projectionMatrix * _viewMatrix; }

        void SetOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near = CAM_NEAR_DEFAULT, const float far = CAM_FAR_DEFAULT);
        void SetOrthographicProjection(const OrthographicProjectionInfo &projInfo);

        void SetPerspectiveProjection(const float fovDeg, const float aspect, const float near = CAM_NEAR_DEFAULT, const float far = CAM_FAR_DEFAULT);
        void SetPerspectiveProjection(const PerspectiveProjectionInfo &projInfo);

        void SetPosition(const glm::vec3 &position);
        void SetRotation(const glm::vec3 &rotation);
        void LookAt(const glm::vec3 target, const glm::vec3 &up = VECTOR3_AXIS_Y);
        void LookAtDirection(const glm::vec3& direction, const glm::vec3 &up = VECTOR3_AXIS_Y);

    private:
        void UpdateViewMatrix();
        void SetViewMatrixUVW(const glm::vec3 &u, const glm::vec3 &v, const glm::vec3 &w);

        bool _isViewMatrixDirty{ true };

        glm::vec3 _position{ 0, 0, 0 };
        glm::vec3 _rotation{ 0, 0, 0 };

        glm::mat4 _projectionMatrix{ 1.f };
        glm::mat4 _viewMatrix      { 1.f };
    };
}
