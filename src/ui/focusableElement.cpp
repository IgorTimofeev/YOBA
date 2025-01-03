#include "focusableElement.h"
#include "application.h"

namespace yoba {
	FocusableElement::~FocusableElement() {
		if (_focusable && isFocused())
			setFocused(false);
	}

	bool FocusableElement::isFocusable() const {
		return _focusable;
	}

	void FocusableElement::setFocusable(bool focusable) {
		_focusable = focusable;
	}

	void FocusableElement::setFocused(bool value) {
		if (!_focusable)
			return;

		const auto app = getApplication();

		if (app)
			app->setFocusedElement(value ? this : nullptr);
	}

	bool FocusableElement::isFocused() {
		const auto app = getApplication();

		return app && app->getFocusedElement() == this;
	}
}