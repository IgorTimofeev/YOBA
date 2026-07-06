#pragma once

#include <string>

namespace YOBA {
	class UTF8 {
		public:
			static uint32_t nextCodepoint(const std::string_view str, size_t& index);
			static uint32_t getLength(const std::string_view str);
	};
}
