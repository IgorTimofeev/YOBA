#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/main/font.h>

namespace YOBA {
	class FontElement : public virtual Element {
		public:
			const Font& getFont() const {
				return *_font;
			}

			void setFont(const Font& value) {
				if (&value == _font)
					return;

				_font = &value;

				invalidate();
			}

		private:
			const Font* _font = nullptr;
	};
}