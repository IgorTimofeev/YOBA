#include "textField.h"

#include "../../../../lib/YOBA/src/ui/keyboard.h"
#include "../../../../lib/YOBA/src/ui/application.h"

namespace yoba {
	void TextField::onRender(ScreenBuffer* screenBuffer) {
		auto primaryColor = getPrimaryColor();

		if (!primaryColor)
			primaryColor = screenBuffer->getPrimaryColor();

		auto secondaryColor = getSecondaryColor();

		if (!secondaryColor)
			secondaryColor = screenBuffer->getSecondaryColor();

		auto& bounds = getBounds();

		screenBuffer->renderFilledRectangle(
			bounds,
			getCornerRadius(),
			primaryColor
		);
	}

	void TextField::onEvent(InputEvent& event) {
		const auto isTouchDown = event.getTypeID() == TouchDownEvent::typeID;
		const auto isTouchDrag = event.getTypeID() == TouchDragEvent::typeID;

		if (!(isTouchDown || isTouchDrag))
			return;

		if (isTouchDown) {
//					setCaptured(true);

			showKeyboard();
		}

		event.setHandled(true);
	}

	void TextField::showKeyboard() {
		auto keyboard = new Keyboard({
			[]() {
				return new EnglishKeyboardLayout();
			}
		});

		keyboard->setKeyHeight(0.08f);
		keyboard->setFont(getKeyboardFont());
		keyboard->setBackgroundColor(getKeyboardBackgroundColor());
		keyboard->setDefaultButtonPrimaryColor(getKeyboardDefaultButtonPrimaryColor());
		keyboard->setDefaultButtonSecondaryColor(getKeyboardDefaultButtonSecondaryColor());
		keyboard->setActionButtonPrimaryColor(getKeyboardActionButtonPrimaryColor());
		keyboard->setActionButtonSecondaryColor(getKeyboardActionButtonSecondaryColor());

		keyboard->setLayoutIndex(0);

		auto root = getApplication();

		auto keyboardAndChildrenLayout = new KeyboardApplicationContainer();
		*keyboardAndChildrenLayout += keyboard;

		auto temporaryRootChildrenLayout = new Container();
		temporaryRootChildrenLayout->setSize(root->getScreenBuffer()->getSize());

		// Moving children from root to temporary layout
		for (auto child : *root)
			*temporaryRootChildrenLayout += child;

		*keyboardAndChildrenLayout += temporaryRootChildrenLayout;

		root->removeChildren();
		*root += keyboardAndChildrenLayout;

		keyboard->getOnKeyDown() += [=](KeyCode code) {
			switch (code) {
				case KeyCode::Enter: {
					root->removeChildren();

					// Moving children back to root
					for (auto child : *temporaryRootChildrenLayout)
						*root += child;

					delete keyboard;
					delete temporaryRootChildrenLayout;
					delete keyboardAndChildrenLayout;

					break;
				}
				default:
					break;
			}
		};
	}

	const Color* TextField::getKeyboardDefaultButtonPrimaryColor() const {
		return _keyboardDefaultButtonPrimaryColor;
	}

	void TextField::setKeyboardDefaultButtonPrimaryColor(const Color* keyboardTextButtonPrimaryColor) {
		_keyboardDefaultButtonPrimaryColor = keyboardTextButtonPrimaryColor;
	}

	const Color* TextField::getKeyboardDefaultButtonSecondaryColor() const {
		return _keyboardDefaultButtonSecondaryColor;
	}

	void TextField::setKeyboardDefaultButtonSecondaryColor(const Color* keyboardTextButtonSecondaryColor) {
		_keyboardDefaultButtonSecondaryColor = keyboardTextButtonSecondaryColor;
	}

	const Color* TextField::getKeyboardActionButtonPrimaryColor() const {
		return _keyboardActionButtonPrimaryColor;
	}

	void TextField::setKeyboardActionButtonPrimaryColor(const Color* keyboardActionButtonPrimaryColor) {
		_keyboardActionButtonPrimaryColor = keyboardActionButtonPrimaryColor;
	}

	const Color* TextField::getKeyboardActionButtonSecondaryColor() const {
		return _keyboardActionButtonSecondaryColor;
	}

	void TextField::setKeyboardActionButtonSecondaryColor(const Color* keyboardActionButtonSecondaryColor) {
		_keyboardActionButtonSecondaryColor = keyboardActionButtonSecondaryColor;
	}

	const Color* TextField::getKeyboardBackgroundColor() const {
		return _keyboardBackgroundColor;
	}

	void TextField::setKeyboardBackgroundColor(const Color* keyboardBackgroundColor) {
		_keyboardBackgroundColor = keyboardBackgroundColor;
	}

	const Font* TextField::getKeyboardFont() const {
		return _keyboardFont;
	}

	void TextField::setKeyboardFont(const Font* keyboardFont) {
		_keyboardFont = keyboardFont;
	}
}