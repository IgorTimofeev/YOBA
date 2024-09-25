#pragma once

#include "functional"
#include "vector"

namespace yoba {
	template <typename ...T>
	class Action {
		public:
			void add(const std::function<void(T...)>& callback) {
				_callbacks.push_back(callback);
			}

			void call(T... args) {
				for (const auto& callback: _callbacks) {
					callback(args...);
				}
			}

			void operator+=(const std::function<void(T...)>& callback) {
				add(callback);
			}

		private:
			std::vector<std::function<void(T...)>> _callbacks {};
	};
}