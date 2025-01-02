#pragma once

#include "ui/element.h"

namespace yoba {
	class TextElement : public virtual Element {
		public:
			const std::u32string_view& getText() const {
				return _text;
			}

			void setText(const std::u32string_view& value) {
				_text = value;

				invalidate();
			}

		protected:
			std::u32string_view _text {};
	};
}
