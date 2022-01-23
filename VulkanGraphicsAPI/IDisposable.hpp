#pragma once
namespace SolEngine::Interface
{
	class IDisposable
	{
	public:
		virtual ~IDisposable() {}
		void virtual Dispose() = 0;
	};
}
