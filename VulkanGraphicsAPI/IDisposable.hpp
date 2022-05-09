#pragma once
namespace SolEngine::Interface
{
	class IDisposable
	{
	public:
		virtual ~IDisposable() = default;
		void virtual Dispose() = 0;
	};
}
