#pragma once

using namespace Utility;

namespace SolEngine
{
	class SolVulkanDevice : public Singleton, public IDisposable
	{
	public:

		// Inherited via IDisposable
		virtual void Dispose() override;
	private:

	};
}
