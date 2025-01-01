#include "keyboard.h"
#include "stackLayout.h"

namespace yoba {
	// ----------------------------- KeyboardKey -----------------------------

	KeyboardKeyType KeyboardKey::getKeyType() const {
		return _keyType;
	}

	KeyboardKey::KeyboardKey(KeyboardKeyType keyType, KeyCode code, const wchar_t* name) :
		_keyType(keyType),
		_code(code),
		_name(name)
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

	// ----------------------------- TextKeyboardKey -----------------------------

	TextKeyboardKey::TextKeyboardKey(KeyCode code, const wchar_t* name, KeyCode uppercaseCode, const wchar_t* uppercaseName) :
		KeyboardKey(KeyboardKeyType::Text, code, name),
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

	KeyboardButton::KeyboardButton(Keyboard* keyboard, KeyboardKey* keyboardKey) : _keyboard(keyboard), _keyboardKey(keyboardKey) {
		setCornerRadius(2);
		setSize(Size(29, 29));
		setText(_keyboardKey->getName());

		if (keyboardKey->getKeyType() == KeyboardKeyType::Text) {
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

	// ----------------------------- KeyboardRow -----------------------------

	KeyboardRow::KeyboardRow() {

	}

	KeyboardRow::~KeyboardRow() {
		for (auto key : keys)
			delete key;
	}

	// ----------------------------- KeyboardLayout -----------------------------

	KeyboardLayoutState::~KeyboardLayoutState() {
		for (auto row : rows)
			delete row;
	}

	KeyboardLayout::KeyboardLayout(const wchar_t* name, const wchar_t* nameAbbreviated) :
		_name(name),
		_nameAbbreviated(nameAbbreviated)
	{

	}

	void KeyboardLayout::setState(KeyboardLayoutState* value) {
		_state = value;
	}

	KeyboardLayout::~KeyboardLayout() {
		delete _state;
	}

	KeyboardLayoutState* KeyboardLayout::getState() const {
		return _state;
	}

	// ----------------------------- Keyboard -----------------------------

	Keyboard::Keyboard(std::vector<std::function<KeyboardLayout*()>> layoutBuilders) : _layoutBuilders(layoutBuilders) {
		*this += &_backgroundPanel;

		_rowsLayout.setMargin(Margin(5));
		_rowsLayout.setSpacing(2);
		*this += &_rowsLayout;
	}

	Keyboard::~Keyboard() {
		deleteLayoutAndUIElements();
	}

	void Keyboard::deleteLayoutAndUIElements() {
		if (!_layout)
			return;

		delete _layout;

		// Deleting existing elements
		for (auto rowElement : _rowsLayout) {
			auto rowStackLayout = dynamic_cast<StackLayout*>(rowElement);

			for (auto button : *rowStackLayout) {
				delete button;
			}

			delete rowStackLayout;
		}
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

		if (!_layout || !_layout->getState())
			return;

		for (auto row : _layout->getState()->rows) {
			auto stackLayout = new StackLayout();
			stackLayout->setOrientation(Orientation::Horizontal);
			stackLayout->setSpacing(2);

			for (auto& key : row->keys) {
				auto keyboardKeyButton = new KeyboardButton(this, key);

				*stackLayout += keyboardKeyButton;
			}

			_rowsLayout += stackLayout;
		}
	}

	int8_t Keyboard::getLayoutIndex() {
		return _layoutIndex;
	}

	// ----------------------------- EnglishKeyboard -----------------------------

	EnglishKeyboardLayoutTextState::EnglishKeyboardLayoutTextState() {
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

		_row3.keys.push_back(&_keyLayout);
		_row3.keys.push_back(&_keyComma);
		_row3.keys.push_back(&_keySpace);
		_row3.keys.push_back(&_keyPeriod);
		_row3.keys.push_back(&_keyHide);
		_row3.keys.push_back(&_keyEnter);
		rows.push_back(&_row3);
	}

	EnglishKeyboardLayout::EnglishKeyboardLayout() : KeyboardLayout(L"English", L"EN") {
		setState(&_textState);
	}
}