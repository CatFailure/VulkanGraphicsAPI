#pragma once
namespace Utility
{
	class Singleton
	{
	public:
		Singleton(const Singleton &) = delete;
		Singleton(Singleton &&) = delete;
		
		void operator=(const Singleton &) = delete;
		void operator=(Singleton &&) = delete;

		static Singleton &GetInstance()
		{
			static Singleton instance;

			return instance;
		}

	private:
		Singleton() {};
	};
}
