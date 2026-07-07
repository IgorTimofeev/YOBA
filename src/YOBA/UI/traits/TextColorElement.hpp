#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/Core/Color.hpp>

namespace YOBA {
	class TextColorElement : public virtual Element {
		public:
			const Color* getTextColor() const {
				return _textColor;
			}

			void setTextColor(const Color* value) {
				if (value == _textColor)
					return;

				_textColor = value;

				invalidate();
			}

		private:
			const Color* _textColor = nullptr;
	};
}
