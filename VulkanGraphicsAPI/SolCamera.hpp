#pragma once
// minwindef has macros for near and far for some reason...
// ... I need them, so disable it!

#pragma push_macro("near")
#pragma push_macro("far")
#undef near
#undef far

namespace SolEngine
{
	class SolCamera
	{
	public:
		/// <summary>
		/// Sets the Camera to use Orthographic Projection.
		/// </summary>
		/// <param name="left">Left boundary plane.</param>
		/// <param name="right">Right boundary plane.</param>
		/// <param name="top">Top boundary plane.</param>
		/// <param name="bottom">Bottom boundary plane.</param>
		/// <param name="near">Near clipping distance.</param>
		/// <param name="far">Far clipping distance.</param>
		void SetOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near, const float far);

		/// <summary>
		/// Sets the Camera to use Perspective Projection.
		/// </summary>
		/// <param name="fovDeg">Vertical Field-of-view in Degrees.</param>
		/// <param name="aspect">Aspect Ratio of window.</param>
		/// <param name="near">Near clipping distance.</param>
		/// <param name="far">Far clipping distance.</param>
		void SetPerspectiveProjection(const float fovDeg, const float aspect, const float near, const float far);

		glm::mat4 GetProjection() const { return _projectionMatrix; }

	private:
		glm::mat4 _projectionMatrix{ 1.f };
	};
}

#pragma pop_macro("near")
#pragma pop_macro("far")