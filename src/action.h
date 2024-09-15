#pragma once

#include "functional"
#include "vector"

namespace yoba {
	template <typename ...T>
	class Action : std::vector<std::function<void(T...)>> {
		public:
			void add(const std::function<void(T...)>& f) {
				this->push_back(f);
			}

			void call(T... args) {
				for (const auto& f: *this) {
					f(args...);
				}
			}
	};
}