#pragma once

#include <functional>
#include <string>

#include <YOBA/core/font.h>

namespace YOBA {
	class Text {
		public:
			static void wrap(
				const Font* font,
				const std::string_view text,
				const uint8_t scale,
				const uint16_t maxWidth,
				const std::function<void(std::string_view, uint16_t width)>& lineHandler
			);
	};
}
