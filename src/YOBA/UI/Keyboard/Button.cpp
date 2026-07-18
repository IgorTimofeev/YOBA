#include <YOBA/UI/Keyboard/Button.hpp>
#include <YOBA/UI/Keyboard/Keyboard.hpp>
#include <YOBA/UI/Keyboard/Controller.hpp>

#include <YOBA/Core/Events/KeyEvent.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(
		const KeyboardKeyType type,
		const Key key,
		const std::string_view name,
		const float width
	) :
		_type(type),
		_key(key),
		_name(name),
		_width(width)
	{

	}

	void KeyboardButton::assignKeyboard(Keyboard* keyboard, const uint8_t row, const uint8_t column) {
		_keyboard = keyboard;
		_row = row;
		_column = column;

		setFocusable(false);
		setCornerRadius(_keyboard->getKeyCornerRadius());
		setFont(_keyboard->getFont());
		updateTextFromCase();
				
		if (canBeEnabled()) {
			switch (_type) {
				case KeyboardKeyType::normal: {
					setDefaultBackgroundColor(_keyboard->getDefaultKeyBackgroundColor());
					setDefaultTextColor(_keyboard->getDefaultKeyTextColor());

					setActiveBackgroundColor(_keyboard->getDefaultKeyTextColor());
					setActiveTextColor(_keyboard->getDefaultKeyBackgroundColor());

					break;
				}
				default: {
					setDefaultBackgroundColor(_keyboard->getActionKeyBackgroundColor());
					setDefaultTextColor(_keyboard->getActionKeyTextColor());

					setActiveBackgroundColor(_keyboard->getActionKeyTextColor());
					setActiveTextColor(_keyboard->getActionKeyBackgroundColor());

					break;
				}
			}
		}
		else {
			setDefaultBackgroundColor(_keyboard->getDisabledKeyBackgroundColor());
			setDefaultTextColor(_keyboard->getDisabledKeyTextColor());
			setEnabled(false);
		}
	}

	void KeyboardButton::onIsActiveChanged() {
		TextButton::onIsActiveChanged();

		const auto key = getKeyFromCase();

		if (key == Key::none)
			return;

		auto active = isActive();
		auto text = getTextFromCase();

		// Copying values here, because invokeLater() will reset it
		Application::getCurrent()->invokeLater([key, text, active] {
			if (active) {
				KeyDownEvent event { key, text };
				Application::getCurrent()->pushEvent(&event);
			}
			else {
				KeyUpEvent event { key, text };
				Application::getCurrent()->pushEvent(&event);
			}
		});
	}

	Key KeyboardButton::getKey() const {
		return _key;
	}

	std::string_view KeyboardButton::getName() const {
		return _name;
	}

	float KeyboardButton::getKeyWidth() const {
		return _width;
	}

	KeyboardKeyType KeyboardButton::getType() const {
		return _type;
	}

	Key KeyboardButton::getKeyFromCase() const {
		return getKey();
	}

	std::string_view KeyboardButton::getNameFromCase() const {
		return getName();
	}

	std::optional<std::string_view> KeyboardButton::getTextFromCase() const {
		return std::nullopt;
	}

	bool KeyboardButton::canBeAdded() {
		return true;
	}

	bool KeyboardButton::canBeEnabled() {
		return true;
	}

	void KeyboardButton::updateTextFromCase() {
		setText(getNameFromCase());
	}

	void KeyboardButton::updateFromCase() {
		updateTextFromCase();
	}

	Keyboard* KeyboardButton::getKeyboard() const {
		return _keyboard;
	}

	uint8_t KeyboardButton::getRow() const {
		return _row;
	}

	uint8_t KeyboardButton::getColumn() const {
		return _column;
	}

	// ----------------------------- TextKeyboardButton -----------------------------

	TextKeyboardButton::TextKeyboardButton(
		const Key key,
		const std::string_view name,
		const Key uppercaseKey,
		const std::string_view uppercaseName,
		const float width
	) :
		KeyboardButton(KeyboardKeyType::normal, key, name, width),
		_uppercaseKey(uppercaseKey),
		_uppercaseName(uppercaseName)
	{

	}

	TextKeyboardButton::TextKeyboardButton(const Key key, const std::string_view name, const float width) :
		TextKeyboardButton(
			key,
			name,
			key,
			name,
			width
		)
	{

	}

	Key TextKeyboardButton::getUppercaseKey() const {
		return _uppercaseKey;
	}

	std::string_view TextKeyboardButton::getUppercaseName() const {
		return _uppercaseName;
	}

	Key TextKeyboardButton::getKeyFromCase() const {
		return getKeyboard()->getCase() == KeyboardCase::lower ? getKey() : getUppercaseKey();
	}

	std::string_view TextKeyboardButton::getNameFromCase() const {
		return getKeyboard()->getCase() == KeyboardCase::lower ? getName() : getUppercaseName();
	}

	std::optional<std::string_view> TextKeyboardButton::getTextFromCase() const {
		return getNameFromCase();
	}

	void TextKeyboardButton::onIsActiveChanged() {
		KeyboardButton::onIsActiveChanged();

		if (isActive())
			return;

		if (getKeyboard()->getCase() == KeyboardCase::upper)
			getKeyboard()->setCase(KeyboardCase::lower);
	}

	// ----------------------------- ActionKeyboardButton -----------------------------

	ShiftKeyboardButton::ShiftKeyboardButton(
		const std::string_view name,
		const std::string_view uppercaseName,
		const std::string_view capsName,
		const float width
	) :
		KeyboardButton(
			KeyboardKeyType::action,
			Key::shift,
			name,
			width
		),
		_uppercaseName(uppercaseName),
		_capsName(capsName)
	{

	}

	void ShiftKeyboardButton::onIsActiveChanged() {
		KeyboardButton::onIsActiveChanged();

		if (isActive())
			return;

		switch (getKeyboard()->getCase()) {
			case KeyboardCase::lower:
				getKeyboard()->setCase(KeyboardCase::upper);
				break;
			case KeyboardCase::upper:
				getKeyboard()->setCase(KeyboardCase::caps);
				break;
			default:
				getKeyboard()->setCase(KeyboardCase::lower);
				break;
		}
	}

	std::string_view ShiftKeyboardButton::getNameFromCase() const {
		switch (getKeyboard()->getCase()) {
			case KeyboardCase::lower:
				return getName();
			case KeyboardCase::upper:
				return _uppercaseName;
			default:
				return _capsName;
		}
	}

	// ----------------------------- BackspaceKeyboardButton -----------------------------

	BackspaceKeyboardButton::BackspaceKeyboardButton(const std::string_view name, const float width) :
		KeyboardButton(KeyboardKeyType::action, Key::backspace, name, width)
	{

	}

	// ----------------------------- EnterKeyboardButton -----------------------------

	EnterKeyboardButton::EnterKeyboardButton(const float width) :
		KeyboardButton(KeyboardKeyType::action, Key::enter, "<-", width)
	{

	}

	void EnterKeyboardButton::onIsActiveChanged() {
		KeyboardButton::onIsActiveChanged();

		if (isActive())
			return;

		Application::getCurrent()->invokeLater([] {
			KeyboardController::hide();
		});
	}

	// ----------------------------- SpaceKeyboardButton -----------------------------

	SpaceKeyboardButton::SpaceKeyboardButton() :TextKeyboardButton(Key::space, " ", Key::space, " ", stretched)
	{

	}

	// ----------------------------- PeriodKeyboardButton -----------------------------

	PeriodKeyboardButton::PeriodKeyboardButton(const float width) : TextKeyboardButton(Key::period, ".", width) {

	}

	bool PeriodKeyboardButton::canBeEnabled() {
		const auto options = getKeyboard()->getLayoutOptions();

		return !(options & KeyboardLayoutOptions::numeric) || options & KeyboardLayoutOptions::allowFractional;
	}

	// ----------------------------- NumericSignMinusKeyboardButton -----------------------------

	NumericSignMinusKeyboardKey::NumericSignMinusKeyboardKey(const float width) : TextKeyboardButton(Key::minus, "-", width) {

	}

	bool NumericSignMinusKeyboardKey::canBeEnabled() {
		const auto options = getKeyboard()->getLayoutOptions();

		return !(options & KeyboardLayoutOptions::numeric) || options & KeyboardLayoutOptions::allowSigned;
	}

	// ----------------------------- NumericSignMinusKeyboardButton -----------------------------

	DummyKeyboardButton::DummyKeyboardButton(const float width) : TextKeyboardButton(Key::none, " ", width) {

	}

	bool DummyKeyboardButton::canBeEnabled() {
		return false;
	}

	// ----------------------------- CharactersLayoutKeyboardButton -----------------------------

	CharactersLayoutKeyboardButton::CharactersLayoutKeyboardButton(
		const std::string_view name,
		const float width
	) :
		KeyboardButton(KeyboardKeyType::charactersLayout, Key::none, name, width)
	{

	}

	void CharactersLayoutKeyboardButton::onIsActiveChanged() {
		KeyboardButton::onIsActiveChanged();

		if (isActive())
			return;

		Application::getCurrent()->invokeLater([this] {
			getKeyboard()->setLayout(getKeyboard()->getCharactersLayoutBuilder()());
		});
	}

	bool CharactersLayoutKeyboardButton::canBeAdded() {
		return !!getKeyboard()->getCharactersLayoutBuilder();
	}

	// ----------------------------- DefaultLayoutKeyboardButton -----------------------------

	CurrentCyclicLayoutKeyboardButton::CurrentCyclicLayoutKeyboardButton(
		const std::string_view name,
		const float width
	) :
		KeyboardButton(KeyboardKeyType::action, Key::none, name, width)
	{

	}

	void CurrentCyclicLayoutKeyboardButton::onIsActiveChanged() {
		KeyboardButton::onIsActiveChanged();

		if (isActive())
			return;

		Application::getCurrent()->invokeLater([this] {
			getKeyboard()->setCyclicLayoutIndex(getKeyboard()->getCyclicLayoutIndex());
		});
	}

	// ----------------------------- CyclicLayoutKeyboardButton -----------------------------

	CyclicLayoutKeyboardButton::CyclicLayoutKeyboardButton(
		const std::string_view name,
		const float width
	) :
		KeyboardButton(KeyboardKeyType::cyclicLayout, Key::none, name, width)
	{

	}

	void CyclicLayoutKeyboardButton::onIsActiveChanged() {
		KeyboardButton::onIsActiveChanged();

		if (isActive())
			return;

		Application::getCurrent()->invokeLater([this] {
			getKeyboard()->setNextCyclicLayoutIndex();
		});
	}

	bool CyclicLayoutKeyboardButton::canBeAdded() {
		return getKeyboard()->getCyclicLayoutBuilders().size() > 1;
	}

}
