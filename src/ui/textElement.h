#pragma once

#include "ui/element.h"

namespace yoba {
	class TextElement : public virtual Element {
		public:
			const wchar_t* getText() const {
				return _text;
			}

			void setText(const wchar_t* value) {
				_text = value;

				invalidate();
			}

		protected:
			const wchar_t* _text = nullptr;
	};
}
