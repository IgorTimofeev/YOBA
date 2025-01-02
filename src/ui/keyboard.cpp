#include "keyboard.h"
#include "stackContainer.h"
#include "../number.h"

namespace yoba {
	// ----------------------------- KeyboardKeyModel -----------------------------

	KeyboardKeyModel::KeyboardKeyModel(KeyboardKeyStyle style, KeyCode code, const std::wstring_view& name, float width) :
		_style(style),
		_code(code),
		_name(name),
		_width(width)
	{

	}

	KeyCode KeyboardKeyModel::getCode() const {
		return _code;
	}

	const std::wstring_view& KeyboardKeyModel::getName() const {
		return _name;
	}

	float KeyboardKeyModel::getWidth() const {
		return _width;
	}

	KeyboardKeyStyle KeyboardKeyModel::getStyle() const {
		return _style;
	}

	KeyCode KeyboardKeyModel::getCodeForCase(Keyboard* keyboard) const {
		return getCode();
	}

	const std::wstring_view& KeyboardKeyModel::getNameFromCase(Keyboard* keyboard) const {
		return getName();
	}

	void KeyboardKeyModel::onClick(Keyboard* keyboard) {
		keyboard->getOnKeyDown().call(getCodeForCase(keyboard));
	}

	// ----------------------------- TextKeyboardKeyModel -----------------------------

	TextKeyboardKeyModel::TextKeyboardKeyModel(KeyCode code, const std::wstring_view& name, KeyCode uppercaseCode, const std::wstring_view& uppercaseName, float width) :
		KeyboardKeyModel(KeyboardKeyStyle::Default, code, name, width),
		_uppercaseCode(uppercaseCode),
		_uppercaseName(uppercaseName)
	{

	}

	TextKeyboardKeyModel::TextKeyboardKeyModel(KeyCode code, const std::wstring_view& name, float width) : TextKeyboardKeyModel(
		code,
		name,
		code,
		name,
		width
	) {

	}

	KeyCode TextKeyboardKeyModel::getUppercaseCode() const {
		return _uppercaseCode;
	}

	const std::wstring_view& TextKeyboardKeyModel::getUppercaseName() const {
		return _uppercaseName;
	}

	KeyCode TextKeyboardKeyModel::getCodeForCase(Keyboard* keyboard) const {
		return keyboard->getLayout()->getCase() ? getUppercaseCode() : getCode();
	}

	const std::wstring_view& TextKeyboardKeyModel::getNameFromCase(Keyboard* keyboard) const {
		return keyboard->getLayout()->getCase() ? getUppercaseName() : getName();
	}

	// ----------------------------- ActionKeyboardKeyModel -----------------------------

	ActionKeyboardKeyModel::ActionKeyboardKeyModel(KeyCode code, const std::wstring_view& name, float width) : KeyboardKeyModel(KeyboardKeyStyle::Action, code, name, width) {

	}

	// ----------------------------- ActionKeyboardKeyModel -----------------------------

	ShiftKeyModel::ShiftKeyModel(KeyCode code, const std::wstring_view& name, float width) : ActionKeyboardKeyModel(code, name, width) {

	}

	void ShiftKeyModel::onClick(Keyboard* keyboard) {
		KeyboardKeyModel::onClick(keyboard);

		keyboard->getLayout()->setCase(!keyboard->getLayout()->getCase());
	}

	// ----------------------------- KeyboardLayout -----------------------------

	bool KeyboardLayout::getCase() const {
		return _case;
	}

	void KeyboardLayout::setCase(bool isShiftDown) {
		_case = isShiftDown;

	}

	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(Keyboard* keyboard, uint8_t rowIndex, uint8_t columnIndex) :
		_keyboard(keyboard),
		_rowIndex(rowIndex),
		_keyIndex(columnIndex)
	{
		setCornerRadius(2);
		updateTextFromCase();

		switch (getKey()->getStyle()) {
			case KeyboardKeyStyle::Default: {
				setPrimaryColor(keyboard->getDefaultButtonPrimaryColor());
				setSecondaryColor(keyboard->getDefaultButtonSecondaryColor());

				setPressedPrimaryColor(keyboard->getDefaultButtonSecondaryColor());
				setPressedSecondaryColor(keyboard->getDefaultButtonPrimaryColor());

				break;
			}
			case KeyboardKeyStyle::Action: {
				setPrimaryColor(keyboard->getActionButtonPrimaryColor());
				setSecondaryColor(keyboard->getActionButtonSecondaryColor());

				setPressedPrimaryColor(keyboard->getActionButtonSecondaryColor());
				setPressedSecondaryColor(keyboard->getActionButtonPrimaryColor());

				break;
			}
		}
	}

	Keyboard* KeyboardButton::getKeyboard() {
		return _keyboard;
	}

	KeyboardKeyModel* KeyboardButton::getKey() {
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

		getKey()->onClick(_keyboard);
	}

	void KeyboardButton::onCaseChanged() {
		updateTextFromCase();
	}

	void KeyboardButton::updateTextFromCase() {
		setText(getKey()->getNameFromCase(getKeyboard()));
	}

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardLayoutRow::KeyboardLayoutRow() {

	}

	// ----------------------------- KeyboardLayout -----------------------------


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

	Callback<KeyCode>& Keyboard::getOnKeyDown() {
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

		_row3.keys.push_back(&_keyCharacters);
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

		_row3.keys.push_back(&_keyCharacters);
		_row3.keys.push_back(&_keyLayout);
		_row3.keys.push_back(&_keyComma);
		_row3.keys.push_back(&_keySpace);
		_row3.keys.push_back(&_keyPeriod);
		_row3.keys.push_back(&_keyEnter);
		rows.push_back(&_row3);
	}
}