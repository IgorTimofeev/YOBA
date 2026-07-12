#pragma once

#include <string>

namespace YOBA {
	class UTF8 {
		public:
			static uint32_t nextCodepoint(const std::string_view str, size_t& index);
			static bool isStartOfNewChar(const char ch);
			static void nextChar(const std::string_view str, size_t& index);
			static uint32_t getLength(const std::string_view str);
			static std::string_view substring(const std::string_view text, const size_t offset, const size_t count = std::string::npos);
	};
}
