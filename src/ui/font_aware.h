#pragma once

#include "element.h"
#include "fonts/font.h"

namespace yoba {
	class FontAware : public virtual Element {
		public:
			Font* getFont() const {
				return _font;
			}

			void setFont(Font* value) {
				_font = value;
			}

		private:
			Font* _font = nullptr;
	};
}