#include "fontElement.h"
#include "YOBA/UI/application.h"

namespace YOBA {
	const Font* FontElement::getFont() const {
		return _font;
	}

	void FontElement::setFont(const Font* value) {
		if (value == _font)
			return;

		_font = value;

		invalidate();
	}
}