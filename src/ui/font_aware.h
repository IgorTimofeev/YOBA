#pragma once

#include "element.h"
#include "fonts/font.h"

namespace yoba {
	class FontAware : public virtual Element {
		public:
			const Font* getFont() const {
				return _font;
			}

			void setFont(const Font* value) {
				_font = value;
			}

		private:
			const Font* _font = nullptr;
	};
}