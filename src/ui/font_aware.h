#pragma once

#include "element.h"

namespace yoba {
	class FontAware : public virtual Element {
		public:
			const uint8_t* getFont() const {
				return _font;
			}

			void setFont(const uint8_t* value) {
				_font = value;
			}

		private:
			const uint8_t* _font = nullptr;
	};
}