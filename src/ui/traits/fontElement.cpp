#include "fontElement.h"
#include "ui/application.h"

namespace yoba::ui {
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