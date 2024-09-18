#include "fontAware.h"
#include "../application.h"

namespace yoba {
	const Font *FontAware::getFont() const {
		return _font;
	}

	void FontAware::setFont(const Font *value) {
		_font = value;

		invalidate();
	}

	const Font *FontAware::getFontOrDefault() {
		return _font ? _font : (getRoot() ? getRoot()->getDefaultFont() : nullptr);
	}
}