#pragma once
namespace Utility
{
	/// <summary>
	/// <para>Used to create Single Instances of an object.</para>
	/// <para>https://stackoverflow.com/a/1008289</para>
	/// </summary>
	template<typename _TyInstance>
	class Singleton
	{
	protected:
		typedef Singleton<_TyInstance> SingletonObject_t;

	public:
		Singleton(const SingletonObject_t&)	     = delete;
		void operator=(const SingletonObject_t&) = delete;

		static _TyInstance& GetInstance()
		{
			static _TyInstance instance;

			return instance;
		}

	protected:
		Singleton() = default;
	};
}
