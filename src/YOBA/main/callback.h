#pragma once

#include "functional"
#include "vector"

namespace YOBA {
	template <typename ...T>
	class Callback {
		public:
			void operator()(T... args) {
				for (const auto& callback : _functions) {
					callback(args...);
				}
			}

			void operator+=(const std::function<void(T...)>& function) {
				_functions.push_back(function);
			}

		private:
			std::vector<std::function<void(T...)>> _functions {};
	};
}