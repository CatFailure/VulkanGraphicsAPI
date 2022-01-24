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
		void SetOrthographicProjection(const float left, const float right, const float top, const float bottom, const float near, const float far);
		void SetPerspectiveProjection(const float fov, const float aspect, const float near, const float far);

		glm::mat4 GetProjection() const { return _projectionMatrix; }

	private:
		glm::mat4 _projectionMatrix{ 1.f };
	};
}

#pragma pop_macro("near")
#pragma pop_macro("far")