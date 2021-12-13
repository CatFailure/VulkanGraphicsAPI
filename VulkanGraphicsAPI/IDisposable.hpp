#pragma once
namespace SolEngine::Interface
{
	class IDisposable
	{
	public:
		void virtual Dispose() = 0;
	};
}
