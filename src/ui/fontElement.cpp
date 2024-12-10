#include "fontElement.h"
#include "ui/application.h"

namespace yoba {
	const Font* FontElement::getFont() const {
		return _font;
	}

	void FontElement::setFont(const Font* value) {
		_font = value;

		invalidate();
	}

	const Font* FontElement::getFontOrDefault() {
		return _font ? _font : (getRoot() ? getRoot()->getFont() : nullptr);
	}
}