#pragma once

#include "fontElement.h"

namespace YOBA {
	class TextElement : public virtual Element {
		public:
			std::wstring_view getText() const {
				return _text;
			}

			void setText(std::wstring_view value) {
				_text = value;

				invalidate();
			}

		private:
			std::wstring _text = std::wstring();
	};
}
