#pragma once

namespace SolEngine::Data
{
	static constexpr float CAM_NEAR_DEFAULT{ 0.01f };
	static constexpr float CAM_FAR_DEFAULT{ 100.f };

	struct PerspectiveProjectionInfo
	{
		float fovDeg;

		float near{ CAM_NEAR_DEFAULT };
		float far{ CAM_FAR_DEFAULT };
	};
}

