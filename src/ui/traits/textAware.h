#pragma once

#include "../element.h"

namespace yoba {
	class TextAware : public virtual Element {
		public:
			const wchar_t* getText() const {
				return _text;
			}

			void setText(const wchar_t* value) {
				_text = value;

				invalidate();
			}

		private:
			const wchar_t* _text = nullptr;
	};
}
