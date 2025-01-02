#include "keyboard.h"
#include "stackContainer.h"
#include "../number.h"

namespace yoba {
	// ----------------------------- KeyboardKey -----------------------------

	KeyboardKeyType KeyboardKey::getKeyType() const {
		return _keyType;
	}

	KeyboardKey::KeyboardKey(KeyboardKeyType keyType, KeyCode code, const std::u32string_view& name, float width) :
		_keyType(keyType),
		_code(code),
		_name(name),
		_width(width)
	{

	}

	KeyCode KeyboardKey::getCode() const {
		return _code;
	}

	const std::u32string_view& KeyboardKey::getName() const {
		return _name;
	}

	float KeyboardKey::getWidth() const {
		return _width;
	}

	// ----------------------------- TextKeyboardKey -----------------------------

	TextKeyboardKey::TextKeyboardKey(KeyCode code, const std::u32string_view& name, KeyCode uppercaseCode, const std::u32string_view& uppercaseName, float width) :
		KeyboardKey(KeyboardKeyType::Text, code, name, width),
		_uppercaseCode(uppercaseCode),
		_uppercaseName(uppercaseName)
	{

	}

	KeyCode TextKeyboardKey::getUppercaseCode() const {
		return _uppercaseCode;
	}

	const std::u32string_view& TextKeyboardKey::getUppercaseName() const {
		return _uppercaseName;
	}

	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(Keyboard* keyboard, uint8_t rowIndex, uint8_t columnIndex) :
		_keyboard(keyboard),
		_rowIndex(rowIndex),
		_keyIndex(columnIndex)
	{
		setCornerRadius(2);
		setText(getKey()->getName());

		if (getKey()->getKeyType() == KeyboardKeyType::Text) {
			setPrimaryColor(keyboard->getTextButtonPrimaryColor());
			setSecondaryColor(keyboard->getTextButtonSecondaryColor());

			setPressedPrimaryColor(keyboard->getTextButtonSecondaryColor());
			setPressedSecondaryColor(keyboard->getTextButtonPrimaryColor());
		}
		else {
			setPrimaryColor(keyboard->getActionButtonPrimaryColor());
			setSecondaryColor(keyboard->getActionButtonSecondaryColor());

			setPressedPrimaryColor(keyboard->getActionButtonSecondaryColor());
			setPressedSecondaryColor(keyboard->getActionButtonPrimaryColor());
		}
	}

	Keyboard* KeyboardButton::getKeyboard() {
		return _keyboard;
	}

	KeyboardKey* KeyboardButton::getKey() {
		return _keyboard->getLayout()->rows[_rowIndex]->keys[_keyIndex];
	}

	uint8_t KeyboardButton::getRowIndex() const {
		return _rowIndex;
	}

	uint8_t KeyboardButton::getKeyIndex() const {
		return _keyIndex;
	}

	void KeyboardButton::onClick() {
		Button::onClick();

		_keyboard->getOnKeyDown().call(getKey());
	}

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardLayoutRow::KeyboardLayoutRow() {

	}

	// ----------------------------- KeyboardLayout -----------------------------

	KeyboardLayout::KeyboardLayout(const std::u32string_view& name, const std::u32string_view& nameAbbreviated) :
		_name(name),
		_nameAbbreviated(nameAbbreviated)
	{

	}

	KeyboardLayout::~KeyboardLayout() {

	}

	// ----------------------------- Keyboard -----------------------------

	Keyboard::Keyboard(std::vector<std::function<KeyboardLayout*()>> layoutBuilders) :
		_layoutBuilders(layoutBuilders)
	{
		*this += &_backgroundPanel;

		_rowsLayout.setSpacing(2);
//		_rowsLayout.setMargin(Margin(5));
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

	const Color* Keyboard::getTextButtonPrimaryColor() const {
		return _textButtonPrimaryColor;
	}

	void Keyboard::setTextButtonPrimaryColor(const Color* value) {
		_textButtonPrimaryColor = value;
	}

	const Color* Keyboard::getTextButtonSecondaryColor() const {
		return _textButtonSecondaryColor;
	}

	void Keyboard::setTextButtonSecondaryColor(const Color* value) {
		_textButtonSecondaryColor = value;
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

	void Keyboard::setLayoutIndex(int8_t value) {
		_layoutIndex = value;

		deleteLayoutAndUIElements();

		_layout = _layoutIndex >= 0 ? _layoutBuilders[_layoutIndex]() : nullptr;

		if (!_layout)
			return;

		for (int rowIndex = 0; rowIndex < _layout->rows.size(); rowIndex++) {
			auto layoutRow = _layout->rows[rowIndex];

			auto UIRow = new KeyboardButtonsRow(this);
			UIRow->setHorizontalAlignment(Alignment::Center);

			for (int keyIndex = 0; keyIndex < layoutRow->keys.size(); keyIndex++) {
				*UIRow += new KeyboardButton(this, rowIndex, keyIndex);
			}

			_rowsLayout += UIRow;
		}
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

	KeyboardLayout* Keyboard::getLayout() const {
		return _layout;
	}

	float Keyboard::getKeyHeight() const {
		return _keyHeight;
	}

	void Keyboard::setKeyHeight(float keyHeight) {
		_keyHeight = keyHeight;
	}

	Callback<KeyboardKey*>& Keyboard::getOnKeyDown() {
		return _onKeyDown;
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

	Size KeyboardRootLayout::computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) {
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

	void KeyboardRootLayout::onArrange(const Bounds& bounds) {
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

	// ----------------------------- EnglishKeyboard -----------------------------

	EnglishKeyboardLayout::EnglishKeyboardLayout() : KeyboardLayout(U"English", U"EN") {
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

		_row3.keys.push_back(&_keyCharacters);
		_row3.keys.push_back(&_keyLayout);
		_row3.keys.push_back(&_keyComma);
		_row3.keys.push_back(&_keySpace);
		_row3.keys.push_back(&_keyPeriod);
		_row3.keys.push_back(&_keyEnter);
		rows.push_back(&_row3);
	}
}