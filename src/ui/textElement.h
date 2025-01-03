#pragma once

#include "fontElement.h"

namespace yoba {
	class TextElement : public virtual FontElement {
		public:
			const std::wstring_view getText() const {
				return _text;
			}

			void setText(const std::wstring_view& value) {
				_text = value;

				invalidate();
			}

		private:
			std::wstring _text {};
	};
}
