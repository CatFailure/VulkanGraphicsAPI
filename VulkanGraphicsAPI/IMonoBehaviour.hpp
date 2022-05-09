#pragma once

namespace SolEngine::Interface
{
	struct IMonoBehaviour
	{
		virtual ~IMonoBehaviour() = default;
		virtual void Update(const float deltaTime) = 0;
	};
}
