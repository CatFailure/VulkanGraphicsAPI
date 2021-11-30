#pragma once
namespace Utility
{
	class IDisposable
	{
	public:
		void virtual Dispose() = 0;
	};
}
