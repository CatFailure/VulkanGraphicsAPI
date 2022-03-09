#pragma once
#include <algorithm>
#include <functional>
#include <list>

namespace SolEngine::Events
{
	/// <summary>
	/// -- A shameless rip-off of the UnityEvent class. --<para />
	/// Allows for methods to be registered/unregistered to an event.<para />
	/// All registered events can then be invoked whenever needed.
	/// </summary>
	/// <typeparam name="...TArgs"></typeparam>
	template<typename ..._TyArgs>
	class SolEvent
	{
	public:
		typedef std::function<void(_TyArgs...)> RegisteredListener_t;
		typedef std::list<RegisteredListener_t>::const_iterator RegisteredListener_cIt;

		/// <summary>
		/// Calls all registered functions to the event.
		/// </summary>
		/// <param name="...args"></param>
		void Invoke(_TyArgs... args) const
		{
			std::for_each(begin(_registeredListeners),
						  end(_registeredListeners),
						  [&args...](const RegisteredListener_t& func)
						  {
						  	  func(args...);
						  });
		}

		/// <summary>
		/// Adds a method to the event to invoke.
		/// </summary>
		/// <param name="func"></param>
		/// <returns></returns>
		RegisteredListener_cIt AddListener(const RegisteredListener_t &func)
		{
			_registeredListeners.push_back(func);

			const RegisteredListener_cIt addedItemIt = std::prev(end(_registeredListeners));

			return addedItemIt;
		}

		/// <summary>
		/// Removes a registered function from the event.
		/// </summary>
		/// <param name="it"></param>
		void RemoveListener(const RegisteredListener_cIt& it)
		{
			if (_registeredListeners.empty())
			{
				return;
			}

			// Since std::functions cannot be compared,
			// they must be removed by iterator instead.
			if (it == end(_registeredListeners))
			{
				return;
			}

			_registeredListeners.erase(it);
		}

		/// <summary>
		/// Removes all registered functions from the event.
		/// </summary>
		void RemoveAllListeners()
		{
			_registeredListeners.clear();
		}

		/// <summary>
		/// Returns how many functions are registered to the event.
		/// </summary>
		/// <returns></returns>
		size_t GetRegisteredListenerCount() const { return _registeredListeners.size(); }

	private:
		std::list<RegisteredListener_t> _registeredListeners;
	};
}
