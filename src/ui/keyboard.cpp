#include "keyboard.h"
#include "stackLayout.h"

namespace yoba {
	// ----------------------------- KeyboardKey -----------------------------

	KeyboardKeyType KeyboardKey::getKeyType() const {
		return _keyType;
	}

	KeyboardKey::KeyboardKey(KeyboardKeyType keyType, KeyCode code, const wchar_t* name, float width) :
		_keyType(keyType),
		_code(code),
		_name(name),
		_width(width)
	{

	}

	KeyboardKey::~KeyboardKey() {

	}

	KeyCode KeyboardKey::getCode() const {
		return _code;
	}

	const wchar_t* KeyboardKey::getName() const {
		return _name;
	}

	float KeyboardKey::getWidth() const {
		return _width;
	}

	// ----------------------------- TextKeyboardKey -----------------------------

	TextKeyboardKey::TextKeyboardKey(KeyCode code, const wchar_t* name, KeyCode uppercaseCode, const wchar_t* uppercaseName, float width) :
		KeyboardKey(KeyboardKeyType::Text, code, name, width),
		_uppercaseCode(uppercaseCode),
		_uppercaseName(uppercaseName)
	{

	}

	KeyCode TextKeyboardKey::getUppercaseCode() const {
		return _uppercaseCode;
	}

	const wchar_t* TextKeyboardKey::getUppercaseName() const {
		return _uppercaseName;
	}


	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(Keyboard* keyboard, uint8_t rowIndex, uint8_t columnIndex) :
		_keyboard(keyboard),
		_rowIndex(rowIndex),
		_keyIndex(columnIndex)
	{
		setCornerRadius(2);
		setText(getKeyboardKey()->getName());

		if (getKeyboardKey()->getKeyType() == KeyboardKeyType::Text) {
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

	KeyboardKey* KeyboardButton::getKeyboardKey() {
		return _keyboard->getLayout()->rows[_rowIndex]->keys[_keyIndex];
	}

	uint8_t KeyboardButton::getRowIndex() const {
		return _rowIndex;
	}

	uint8_t KeyboardButton::getKeyIndex() const {
		return _keyIndex;
	}

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardLayoutRow::KeyboardLayoutRow() {

	}

	KeyboardLayoutRow::~KeyboardLayoutRow() {
		for (auto key : keys)
			delete key;
	}

	// ----------------------------- KeyboardLayout -----------------------------

	KeyboardLayout::KeyboardLayout(const wchar_t* name, const wchar_t* nameAbbreviated) :
		_name(name),
		_nameAbbreviated(nameAbbreviated)
	{

	}

	KeyboardLayout::~KeyboardLayout() {
		for (auto row : rows)
			delete row;
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

		delete _layout;

		for (auto element : _rowsLayout) {
			auto row = dynamic_cast<KeyboardUIRow*>(element);

			delete row;
		}

		_rowsLayout.removeChildren();
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

			auto UIRow = new KeyboardUIRow(this);
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

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardUIRow::KeyboardUIRow(Keyboard* keyboard) : _keyboard(keyboard) {

	}

	Keyboard* KeyboardUIRow::getKeyboard() const {
		return _keyboard;
	}

	Size KeyboardUIRow::computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) {
		const auto spacingTotalWidth = _keyboard->getHorizontalKeySpacing() * (getChildrenCount() - 1);
		const auto availableWidthWithoutSpacing = availableSize.getWidth() - spacingTotalWidth;

		float floatButtonWidth;
		float floatWidth = 0;

		auto buttonHeight = (uint16_t) (_keyboard->getKeyHeight() * availableSize.getWidth());

		for (auto child : *this) {
			if (!child->isVisible())
				continue;

			auto button = dynamic_cast<KeyboardButton*>(child);

			floatButtonWidth = button->getKeyboardKey()->getWidth() * availableWidthWithoutSpacing;

			button->measure(
				screenBuffer,
				Size(
					(uint16_t) floatButtonWidth,
					buttonHeight
				)
			);

			floatWidth += floatButtonWidth + _keyboard->getHorizontalKeySpacing();
		}

		return Size(
			floatWidth - _keyboard->getHorizontalKeySpacing(),
			buttonHeight
		);
	}

	void KeyboardUIRow::onArrange(const Bounds& bounds) {
		auto x = bounds.getX();

		const auto spacingTotalWidth = _keyboard->getHorizontalKeySpacing() * (getChildrenCount() - 1);
		const auto availableWidthWithoutSpacing = bounds.getWidth() - spacingTotalWidth;

		for (auto child : *this) {
			if (!child->isVisible())
				continue;

			auto button = dynamic_cast<KeyboardButton*>(child);

			const auto& buttonSize = button->getMeasuredSize();

			button->arrange(Bounds(
				(int32_t) x,
				bounds.getY(),
				buttonSize.getWidth(),
				buttonSize.getHeight()
			));

			x += buttonSize.getWidth() + _keyboard->getHorizontalKeySpacing();
		}
	}

	// ----------------------------- EnglishKeyboard -----------------------------

	EnglishKeyboardLayout::EnglishKeyboardLayout() : KeyboardLayout(L"English", L"EN") {
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