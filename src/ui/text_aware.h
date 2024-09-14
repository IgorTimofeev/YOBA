#pragma once

#include "element.h"

namespace yoba {
	class TextAware : public virtual Element {
		public:
			const String &getText() const {
				return _text;
			}

			void setText(const String &value) {
				_text = value;
			}

		private:
			String _text = "Hello";
	};
}
