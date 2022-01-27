#pragma once
// minwindef has macros for near and far for some reason...
// ... I need them, so disable it!

#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far

using namespace Utility;

namespace SolEngine
{
    class SolCamera
    {
    public:
        /// <summary>
        /// Sets the Camera to use Orthographic Projection.
        /// </summary>
        /// <param name="left"> Left boundary plane. </param>
        /// <param name="right"> Right boundary plane. </param>
        /// <param name="top"> Top boundary plane. </param>
        /// <param name="bottom">Bottom boundary plane. </param>
        /// <param name="near"> Near clipping distance. </param>
        /// <param name="far"> Far clipping distance. </param>
        void SetOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near, const float far);

        /// <summary>
        /// Sets the Camera to use Perspective Projection.
        /// </summary>
        /// <param name="fovDeg"> Vertical Field-of-view in Degrees. </param>
        /// <param name="aspect"> Aspect Ratio of window. </param>
        /// <param name="near"> Near clipping distance. </param>
        /// <param name="far"> Far clipping distance. </param>
        void SetPerspectiveProjection(const float fovDeg, const float aspect, const float near, const float far);

        /// <summary>
        /// Points camera in a desired direction.
        /// </summary>
        void SetViewDirection(const glm::vec3 &position, const glm::vec3 &direction, const glm::vec3 &up = glm::vec3(0.f, -1.f, 0.f));

        /// <summary>
        /// Points camera towards a specific point.
        /// </summary>
        void LookAt(const glm::vec3 &position, const glm::vec3 &target, const glm::vec3 &up = glm::vec3(0.f, -1.f, 0.f));

        /// <summary>
        /// Sets camera transform - Rotation follows Tait-Bryan Y(1), X(2), Z(3)
        /// </summary>
        void SetView(const glm::vec3& position, const glm::vec3& rotation);

        glm::mat4 GetProjection()     const { return _projectionMatrix; }
        glm::mat4 GetView()		      const { return _viewMatrix; }
        glm::mat4 GetProjectionView() const { return _projectionMatrix * _viewMatrix; }

    private:
        glm::mat4 _projectionMatrix{ 1.f };
        glm::mat4 _viewMatrix{ 1.f };	// Stores the Camera's transform
    };
}

#pragma pop_macro("near")
#pragma pop_macro("far")