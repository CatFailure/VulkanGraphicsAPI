#pragma once

namespace SolEngine::Interface
{
	class IMonoBehaviour
	{
	public:
		virtual void Update(const float deltaTime) = 0;
	};
}
