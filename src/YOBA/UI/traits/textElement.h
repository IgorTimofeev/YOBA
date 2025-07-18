#pragma once

#include <YOBA/UI/element.h>

namespace YOBA {
	class TextElement : public virtual Element {
		public:
			std::wstring_view getText() const {
				return text;
			}

			void setText(const std::wstring_view value) {
				if (value == text)
					return;

				text = value;

				onTextChanged();

				invalidate();
			}

		protected:
			virtual void onTextChanged() {

			}

		private:
			std::wstring text = std::wstring();
	};
}
