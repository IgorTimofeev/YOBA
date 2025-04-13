#include "focusableElement.h"
#include "ui/application.h"
#include <esp_log.h>

namespace yoba::ui {
	void FocusableElement::setApplication(Application* value) {
		ESP_LOGI("FocusableElement", "setApplication(), isFocused: %d", isFocused());

		setFocused(false);

		Element::setApplication(value);
	}

	bool FocusableElement::isFocusable() const {
		return _focusable;
	}

	void FocusableElement::setFocusable(bool focusable) {
		_focusable = focusable;
	}

	void FocusableElement::setFocused(bool value) {
		ESP_LOGI("FocusableElement", "setFocused(%d)", value);

		if (!_focusable || isFocused() == value)
			return;

		const auto app = getApplication();

		if (app == nullptr || app->getFocusedElement() == this)
			return;

		app->setFocusedElement(value ? this : nullptr);
	}

	bool FocusableElement::isFocused() {
		const auto app = getApplication();

		return app && app->getFocusedElement() == this;
	}
}