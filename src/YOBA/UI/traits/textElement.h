#pragma once

#include <YOBA/UI/element.h>

namespace YOBA {
	class TextElement : public virtual Element {
		public:
			Callback<> textChanged {};

			std::wstring_view getText() const {
				return _text;
			}

			void setText(std::wstring_view value) {
				_text = value;

				onTextChanged();
				textChanged();

				invalidate();
			}

		protected:
			virtual void onTextChanged() {

			}

		private:
			std::wstring _text = std::wstring();
	};
}
