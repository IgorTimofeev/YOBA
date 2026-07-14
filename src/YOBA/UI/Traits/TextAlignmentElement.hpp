#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/Core/Alignment.hpp>

namespace YOBA {
	class TextAlignmentElement : public virtual Element {
		public:
			Alignment getTextAlignment() const {
				return _textAlignment;
			}

			void setTextAlignment(const Alignment value) {
				if (value == _textAlignment)
					return;

				_textAlignment = value;

				invalidate();
			}

		private:
			Alignment _textAlignment = Alignment::start;
	};
}
