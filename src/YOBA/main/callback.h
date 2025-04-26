#pragma once

#include "functional"
#include "vector"

namespace YOBA {
	template <typename ...T>
	class Callback {
		public:
			void operator()(T... args);
			void operator+=(const std::function<void(T...)>& function);
			void operator-=(const std::function<void(T...)>& function);

		private:
			std::vector<std::function<void(T...)>> _functions {};
	};

	template<typename... T>
	void Callback<T...>::operator-=(const std::function<void(T...)>& function) {
		_functions.erase(function);
	}

	template<typename... T>
	void Callback<T...>::operator+=(const std::function<void(T...)>& function) {
		_functions.push_back(function);
	}

	template<typename... T>
	void Callback<T...>::operator()(T... args) {
		for (const auto& callback : _functions) {
			callback(args...);
		}
	}
}