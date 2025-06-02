#pragma once

#include "functional"
#include "vector"

namespace YOBA {
	template <typename ...T>
	class Callback {
		public:
			void operator()(T... args);
			size_t operator+=(const std::function<void(T...)>& function);
			void operator-=(size_t index);

		private:
			std::vector<std::function<void(T...)>> _functions {};
	};

	template<typename... T>
	size_t Callback<T...>::operator+=(const std::function<void(T...)>& function) {
		_functions.push_back(function);

		return _functions.size() - 1;
	}

	template <typename ... T>
	void Callback<T...>::operator-=(size_t index) {
		_functions.erase(_functions.begin() + index);
	}

	template<typename... T>
	void Callback<T...>::operator()(T... args) {
		for (const auto& callback : _functions) {
			callback(args...);
		}
	}
}