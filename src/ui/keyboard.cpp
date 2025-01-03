#include <cstdint>
#include "keyboard.h"
#include "stackContainer.h"
#include "../number.h"

namespace yoba {
	// ----------------------------- KeyboardKey -----------------------------

	KeyboardKey::KeyboardKey(KeyboardKeyType type, KeyCode code, const std::wstring_view& name, float width) :
		_type(type),
		_code(code),
		_name(name),
		_width(width)
	{

	}

	KeyCode KeyboardKey::getCode() const {
		return _code;
	}

	const std::wstring_view& KeyboardKey::getName() const {
		return _name;
	}

	float KeyboardKey::getWidth() const {
		return _width;
	}

	KeyboardKeyType KeyboardKey::getType() const {
		return _type;
	}

	KeyCode KeyboardKey::getCodeFromCase(Keyboard* keyboard) const {
		return getCode();
	}

	const std::wstring_view& KeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		return getName();
	}

	void KeyboardKey::onPress(Keyboard* keyboard) {
		const auto code = getCodeFromCase(keyboard);

		if (code != KeyCode::None)
			keyboard->getOnKeyPress().call(code);
	}

	bool KeyboardKey::isContinuousTypingEnabled() const {
		return _continuousTypingEnabled;
	}

	void KeyboardKey::setContinuousTypingEnabled(bool value) {
		_continuousTypingEnabled = value;
	}

	// ----------------------------- TextKeyboardKey -----------------------------

	TextKeyboardKey::TextKeyboardKey(KeyCode code, const std::wstring_view& name, KeyCode uppercaseCode, const std::wstring_view& uppercaseName, float width) :
		KeyboardKey(KeyboardKeyType::Default, code, name, width),
		_uppercaseCode(uppercaseCode),
		_uppercaseName(uppercaseName)
	{
		setContinuousTypingEnabled(true);
	}

	TextKeyboardKey::TextKeyboardKey(KeyCode code, const std::wstring_view& name, float width) : TextKeyboardKey(
		code,
		name,
		code,
		name,
		width
	) {

	}

	KeyCode TextKeyboardKey::getUppercaseCode() const {
		return _uppercaseCode;
	}

	const std::wstring_view& TextKeyboardKey::getUppercaseName() const {
		return _uppercaseName;
	}

	void TextKeyboardKey::onPress(Keyboard* keyboard) {
		KeyboardKey::onPress(keyboard);

		const auto keyboardCase = keyboard->getCase();

		keyboard->getOnInput().call(getCodeFromCase(keyboard), getNameFromCase(keyboard));

		if (keyboardCase == KeyboardCase::Upper)
			keyboard->setCase(KeyboardCase::Lower);
	}

	KeyCode TextKeyboardKey::getCodeFromCase(Keyboard* keyboard) const {
		return keyboard->getCase() == KeyboardCase::Lower ? getCode() : getUppercaseCode();
	}

	const std::wstring_view& TextKeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		return keyboard->getCase() == KeyboardCase::Lower ? getName() : getUppercaseName();
	}

	// ----------------------------- ActionKeyboardKey -----------------------------

	ActionKeyboardKey::ActionKeyboardKey(KeyCode code, const std::wstring_view& name, float width) : KeyboardKey(KeyboardKeyType::Action, code, name, width) {

	}

	// ----------------------------- ActionKeyboardKey -----------------------------

	ShiftKeyboardKey::ShiftKeyboardKey(
		const std::wstring_view& name,
		const std::wstring_view& uppercaseName,
		const std::wstring_view& capsName,
		float width
	) :
		ActionKeyboardKey(
			KeyCode::Shift,
			name,
			width
		),
		_uppercaseName(uppercaseName),
		_capsName(capsName)
	{

	}

	void ShiftKeyboardKey::onPress(Keyboard* keyboard) {
		KeyboardKey::onPress(keyboard);

		switch (keyboard->getCase()) {
			case KeyboardCase::Lower:
				keyboard->setCase(KeyboardCase::Upper);
				break;
			case KeyboardCase::Upper:
				keyboard->setCase(KeyboardCase::Caps);
				break;
			default:
				keyboard->setCase(KeyboardCase::Lower);
				break;
		}
	}

	const std::wstring_view& ShiftKeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		switch (keyboard->getCase()) {
			case KeyboardCase::Lower:
				return getName();
			case KeyboardCase::Upper:
				return _uppercaseName;
			default:
				return _capsName;
		}
	}

	// ----------------------------- BackspaceKeyboardKey -----------------------------

	BackspaceKeyboardKey::BackspaceKeyboardKey(const std::wstring_view& name, float width) : ActionKeyboardKey(KeyCode::Backspace, name, width) {
		setContinuousTypingEnabled(true);
	}

	// ----------------------------- CharactersLayoutKeyboardKey -----------------------------

	CharactersLayoutKeyboardKey::CharactersLayoutKeyboardKey(
		const std::wstring_view& name,
		float width,
		const std::function<KeyboardLayout*()>& layoutBuilder
	) :
		ActionKeyboardKey(KeyCode::None, name, width),
		_layoutBuilder(layoutBuilder)
	{

	}

	void CharactersLayoutKeyboardKey::onPress(Keyboard* keyboard) {
		KeyboardKey::onPress(keyboard);

		keyboard->setLayout(_layoutBuilder());
	}

	// ----------------------------- CyclicLayoutKeyboardKey -----------------------------

	CyclicLayoutKeyboardKey::CyclicLayoutKeyboardKey(
		const std::wstring_view& name,
		float width
	) :
		ActionKeyboardKey(KeyCode::None, name, width)
	{

	}

	void CyclicLayoutKeyboardKey::onPress(Keyboard* keyboard) {
		KeyboardKey::onPress(keyboard);

		keyboard->setLayoutIndex(keyboard->getLayoutIndex());
	}

	// ----------------------------- KeyboardLayout -----------------------------



	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(Keyboard* keyboard, KeyboardKey* key) :
		_keyboard(keyboard),
		_key(key)
	{
		setFocusable(false);
		setCornerRadius(2);
		setFont(_keyboard->getFont());
		updateTextFromCase();

		switch (_key->getType()) {
			case KeyboardKeyType::Default: {
				setPrimaryColor(_keyboard->getDefaultButtonPrimaryColor());
				setSecondaryColor(_keyboard->getDefaultButtonSecondaryColor());

				setPressedPrimaryColor(_keyboard->getDefaultButtonSecondaryColor());
				setPressedSecondaryColor(_keyboard->getDefaultButtonPrimaryColor());

				break;
			}
			case KeyboardKeyType::Action: {
				setPrimaryColor(_keyboard->getActionButtonPrimaryColor());
				setSecondaryColor(_keyboard->getActionButtonSecondaryColor());

				setPressedPrimaryColor(_keyboard->getActionButtonSecondaryColor());
				setPressedSecondaryColor(_keyboard->getActionButtonPrimaryColor());

				break;
			}
		}
	}

	void KeyboardButton::tick() {
		Element::tick();

		if (!_key->isContinuousTypingEnabled() || !isPressed() || millis() < _continuousTypingTime)
			return;

		_key->onPress(_keyboard);

		_continuousTypingTime = millis() + _keyboard->getContinuousTypingInterval();
	}

	void KeyboardButton::onPressedChanged() {
		Button::onPressedChanged();

		if (!isPressed())
			return;

		_key->onPress(_keyboard);

		if (!_key->isContinuousTypingEnabled())
			return;

		_continuousTypingTime = millis() + _keyboard->getContinuousTypingDelay();
	}

	void KeyboardButton::updateTextFromCase() {
		setText(_key->getNameFromCase(getKeyboard()));
	}

	void KeyboardButton::updateFromCase() {
		updateTextFromCase();
	}

	Keyboard* KeyboardButton::getKeyboard() {
		return _keyboard;
	}

	KeyboardKey* KeyboardButton::getKey() {
		return _key;
	}

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardLayoutRow::KeyboardLayoutRow() {

	}

	// ----------------------------- KeyboardLayout -----------------------------


	// ----------------------------- Keyboard -----------------------------

	Keyboard::Keyboard(std::vector<std::function<KeyboardLayout*()>> cyclicLayoutBuilders) :
		_cyclicLayoutBuilders(cyclicLayoutBuilders)
	{
		*this += &_backgroundPanel;

		_rowsLayout.setSpacing(2);
		*this += &_rowsLayout;
	}

	Keyboard::~Keyboard() {
		deleteLayoutAndUIElements();
	}

	void Keyboard::deleteLayoutAndUIElements() {
		if (!_layout)
			return;

		for (auto element : _rowsLayout) {
			delete dynamic_cast<KeyboardButtonsRow*>(element);
		}

		_rowsLayout.removeChildren();

		delete _layout;
	}

	const Color* Keyboard::getBackgroundColor() const {
		return _backgroundPanel.getPrimaryColor();
	}

	void Keyboard::setBackgroundColor(const Color* value) {
		_backgroundPanel.setPrimaryColor(value);
	}

	const Color* Keyboard::getDefaultButtonPrimaryColor() const {
		return _defaultButtonPrimaryColor;
	}

	void Keyboard::setDefaultButtonPrimaryColor(const Color* value) {
		_defaultButtonPrimaryColor = value;
	}

	const Color* Keyboard::getDefaultButtonSecondaryColor() const {
		return _defaultButtonSecondaryColor;
	}

	void Keyboard::setDefaultButtonSecondaryColor(const Color* value) {
		_defaultButtonSecondaryColor = value;
	}

	const Color* Keyboard::getActionButtonPrimaryColor() const {
		return _actionButtonPrimaryColor;
	}

	void Keyboard::setActionButtonPrimaryColor(const Color* value) {
		_actionButtonPrimaryColor = value;
	}

	const Color* Keyboard::getActionButtonSecondaryColor() const {
		return _actionButtonSecondaryColor;
	}

	void Keyboard::setActionButtonSecondaryColor(const Color* actionButtonSecondaryColor) {
		_actionButtonSecondaryColor = actionButtonSecondaryColor;
	}

	KeyboardLayout* Keyboard::getLayout() const {
		return _layout;
	}

	void Keyboard::setLayout(KeyboardLayout* value) {
		deleteLayoutAndUIElements();

		_case = KeyboardCase::Lower;
		_layout = value;

		if (!_layout)
			return;

		for (int rowIndex = 0; rowIndex < _layout->rows.size(); rowIndex++) {
			auto layoutRow = _layout->rows[rowIndex];

			auto UIRow = new KeyboardButtonsRow(this);
			UIRow->setHorizontalAlignment(Alignment::Center);

			for (int keyIndex = 0; keyIndex < layoutRow->keys.size(); keyIndex++) {
				*UIRow += new KeyboardButton(this, layoutRow->keys[keyIndex]);
			}

			_rowsLayout += UIRow;
		}
	}

	void Keyboard::setLayoutIndex(int8_t value) {
		_layoutIndex = value;

		setLayout(_layoutIndex >= 0 ? _cyclicLayoutBuilders[_layoutIndex]() : nullptr);
	}

	int8_t Keyboard::getLayoutIndex() {
		return _layoutIndex;
	}

	uint8_t Keyboard::getHorizontalKeySpacing() const {
		return _horizontalKeySpacing;
	}

	void Keyboard::setHorizontalKeySpacing(uint8_t horizontalKeySpacing) {
		_horizontalKeySpacing = horizontalKeySpacing;
	}

	uint8_t Keyboard::getVerticalKeySpacing() const {
		return _rowsLayout.getSpacing();
	}

	void Keyboard::setVerticalKeySpacing(uint8_t value) {
		_rowsLayout.setSpacing(value);
	}

	float Keyboard::getKeyHeight() const {
		return _keyHeight;
	}

	void Keyboard::setKeyHeight(float keyHeight) {
		_keyHeight = keyHeight;
	}

	KeyboardCase Keyboard::getCase() const {
		return _case;
	}

	void Keyboard::setCase(KeyboardCase value) {
		_case = value;

		if (!_layout)
			return;

		iterateOverButtons([](KeyboardButton* button) {
			button->updateFromCase();
		});
	}

	void Keyboard::iterateOverButtons(std::function<void(KeyboardButton*)> handler) {
		for (auto rowElement : _rowsLayout) {
			auto row = dynamic_cast<KeyboardButtonsRow*>(rowElement);

			for (auto buttonElement : *row) {
				handler(dynamic_cast<KeyboardButton*>(buttonElement));
			}
		}
	}

	Callback<KeyCode>& Keyboard::getOnKeyPress() {
		return _onKeyPress;
	}

	Callback<KeyCode, const std::wstring_view&>& Keyboard::getOnInput() {
		return _onInput;
	}

	uint16_t Keyboard::getContinuousTypingDelay() const {
		return _continuousTypingDelay;
	}

	void Keyboard::setContinuousTypingDelay(uint16_t value) {
		_continuousTypingDelay = value;
	}

	uint16_t Keyboard::getContinuousTypingInterval() const {
		return _continuousTypingInterval;
	}

	void Keyboard::setContinuousTypingInterval(uint16_t value) {
		_continuousTypingInterval = value;
	}

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardButtonsRow::KeyboardButtonsRow(Keyboard* keyboard) : _keyboard(keyboard) {

	}

	KeyboardButtonsRow::~KeyboardButtonsRow() {
		for (auto child : *this) {
			delete dynamic_cast<KeyboardButton*>(child);
		}
	}

	Keyboard* KeyboardButtonsRow::getKeyboard() const {
		return _keyboard;
	}

	Size KeyboardButtonsRow::computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) {
		const auto availableWidthWithoutSpacing = availableSize.getWidth() - _keyboard->getHorizontalKeySpacing() * (getChildrenCount() - 1);

		float resultWidth = 0;

		const auto buttonHeight = (uint16_t) (_keyboard->getKeyHeight() * availableSize.getWidth());

		for (auto child : *this) {
			auto button = dynamic_cast<KeyboardButton*>(child);

			const auto buttonWidth = button->getKey()->getWidth() * availableWidthWithoutSpacing;

			button->measure(
				screenBuffer,
				Size(
					std::round(buttonWidth),
					buttonHeight
				)
			);

			resultWidth += buttonWidth + _keyboard->getHorizontalKeySpacing();
		}

		return Size(
			std::round(resultWidth - _keyboard->getHorizontalKeySpacing()),
			buttonHeight
		);
	}

	void KeyboardButtonsRow::onArrange(const Bounds& bounds) {
		const auto availableWidthWithoutSpacing = bounds.getWidth() - _keyboard->getHorizontalKeySpacing() * (getChildrenCount() - 1);

		float widthSum = 0;

		for (auto child : *this) {
			widthSum += dynamic_cast<KeyboardButton*>(child)->getKey()->getWidth();
		}

		float x = bounds.getX();

		for (auto child : *this) {
			auto button = dynamic_cast<KeyboardButton*>(child);

			const float buttonWidth = button->getKey()->getWidth() / widthSum * availableWidthWithoutSpacing;

			button->arrange(Bounds(
				std::round(x),
				bounds.getY(),
				std::round(buttonWidth),
				bounds.getHeight()
			));

			x += buttonWidth + _keyboard->getHorizontalKeySpacing();
		}
	}

	// ----------------------------- KeyboardRootLayout -----------------------------

	Size KeyboardApplicationContainer::computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) {
		auto result = Size();

		for (auto child : *this) {
			child->measure(
				screenBuffer,
				Size(
					availableSize.getWidth(),
					Size::Infinity
				)
			);

			const auto& childSize = child->getMeasuredSize();

			if (childSize.getWidth() > result.getWidth())
				result.setWidth(childSize.getWidth());

			result.setHeight(result.getHeight() + childSize.getHeight());
		}

		return result;
	}

	void KeyboardApplicationContainer::onArrange(const Bounds& bounds) {
		auto y = bounds.getY2() + 1;

		for (auto child : *this) {
			y -= child->getMeasuredSize().getHeight();

			child->arrange(Bounds(
				bounds.getX(),
				y,
				bounds.getWidth(),
				child->getMeasuredSize().getHeight()
			));
		}
	}

	// ----------------------------- EnglishKeyboardLayout -----------------------------

	EnglishKeyboardLayout::EnglishKeyboardLayout() {
		_row0.keys.push_back(&_keyQ);
		_row0.keys.push_back(&_keyW);
		_row0.keys.push_back(&_keyE);
		_row0.keys.push_back(&_keyR);
		_row0.keys.push_back(&_keyT);
		_row0.keys.push_back(&_keyY);
		_row0.keys.push_back(&_keyU);
		_row0.keys.push_back(&_keyI);
		_row0.keys.push_back(&_keyO);
		_row0.keys.push_back(&_keyP);
		rows.push_back(&_row0);

		_row1.keys.push_back(&_keyA);
		_row1.keys.push_back(&_keyS);
		_row1.keys.push_back(&_keyD);
		_row1.keys.push_back(&_keyF);
		_row1.keys.push_back(&_keyG);
		_row1.keys.push_back(&_keyH);
		_row1.keys.push_back(&_keyJ);
		_row1.keys.push_back(&_keyK);
		_row1.keys.push_back(&_keyL);
		rows.push_back(&_row1);

		_row2.keys.push_back(&_keyShift);
		_row2.keys.push_back(&_keyZ);
		_row2.keys.push_back(&_keyX);
		_row2.keys.push_back(&_keyC);
		_row2.keys.push_back(&_keyV);
		_row2.keys.push_back(&_keyB);
		_row2.keys.push_back(&_keyN);
		_row2.keys.push_back(&_keyM);
		_row2.keys.push_back(&_keyBackspace);
		rows.push_back(&_row2);

		_row3.keys.push_back(&_keyCharactersLayout);
		_row3.keys.push_back(&_keyLayout);
		_row3.keys.push_back(&_keyComma);
		_row3.keys.push_back(&_keySpace);
		_row3.keys.push_back(&_keyPeriod);
		_row3.keys.push_back(&_keyEnter);
		rows.push_back(&_row3);
	}

	// ----------------------------- NumericKeyboardLayout -----------------------------

	CharactersKeyboardLayout::CharactersKeyboardLayout() {
		_row0.keys.push_back(&_key1);
		_row0.keys.push_back(&_key2);
		_row0.keys.push_back(&_key3);
		_row0.keys.push_back(&_key4);
		_row0.keys.push_back(&_key5);
		_row0.keys.push_back(&_key6);
		_row0.keys.push_back(&_key7);
		_row0.keys.push_back(&_key8);
		_row0.keys.push_back(&_key9);
		_row0.keys.push_back(&_key0);
		rows.push_back(&_row0);

		_row1.keys.push_back(&_keyAt);
		_row1.keys.push_back(&_keyNumberSign);
		_row1.keys.push_back(&_keyDollar);
		_row1.keys.push_back(&_keyUnderscore);
		_row1.keys.push_back(&_keyAmpersand);
		_row1.keys.push_back(&_keyMinus);
		_row1.keys.push_back(&_keyPlus);
		_row1.keys.push_back(&_keyLeftBrace);
		_row1.keys.push_back(&_keyRightBrace);
		_row1.keys.push_back(&_keySlash);
		rows.push_back(&_row1);

		_row2.keys.push_back(&_keyAsterisk);
		_row2.keys.push_back(&_keyDoubleQuote);
		_row2.keys.push_back(&_keyQuote);
		_row2.keys.push_back(&_keyColon);
		_row2.keys.push_back(&_keySemicolon);
		_row2.keys.push_back(&_keyExclamationMark);
		_row2.keys.push_back(&_keyQuestionMark);
		_row2.keys.push_back(&_keyBackspace);
		rows.push_back(&_row2);

		_row3.keys.push_back(&_keyCyclicLayout);
		_row3.keys.push_back(&_keyLayout);
		_row3.keys.push_back(&_keyComma);
		_row3.keys.push_back(&_keySpace);
		_row3.keys.push_back(&_keyPeriod);
		_row3.keys.push_back(&_keyEnter);
		rows.push_back(&_row3);
	}
}