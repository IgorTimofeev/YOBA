#include <cstdint>
#include "keyboard.h"
#include "stackContainer.h"
#include "application.h"
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

	float KeyboardKey::fit = -1;

	void KeyboardKey::tick(KeyboardButton* button) {

	}

	void KeyboardKey::onPressedChanged(KeyboardButton* button) {
		const auto keyboard = button->getKeyboard();
		const auto code = getCodeFromCase(keyboard);

		if (code != KeyCode::None)
			keyboard->getOnKeyPressedChanged()(code, button->isPressed());
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

	// ----------------------------- TextKeyboardKey -----------------------------

	TextKeyboardKey::TextKeyboardKey(
		KeyCode code,
		const std::wstring_view& name,
		KeyCode uppercaseCode,
		const std::wstring_view& uppercaseName,
		float width
	) :
		KeyboardKey(KeyboardKeyType::Default, code, name, width),
		_uppercaseCode(uppercaseCode),
		_uppercaseName(uppercaseName)
	{

	}

	TextKeyboardKey::TextKeyboardKey(KeyCode code, const std::wstring_view& name, float width) :
		TextKeyboardKey(
			code,
			name,
			code,
			name,
			width
		)
	{

	}

	KeyCode TextKeyboardKey::getUppercaseCode() const {
		return _uppercaseCode;
	}

	const std::wstring_view& TextKeyboardKey::getUppercaseName() const {
		return _uppercaseName;
	}

	KeyCode TextKeyboardKey::getCodeFromCase(Keyboard* keyboard) const {
		return keyboard->getCase() == KeyboardCase::Lower ? getCode() : getUppercaseCode();
	}

	const std::wstring_view& TextKeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		return keyboard->getCase() == KeyboardCase::Lower ? getName() : getUppercaseName();
	}

	void TextKeyboardKey::onPressedChanged(KeyboardButton* button) {
		KeyboardKey::onPressedChanged(button);

		if (button->isPressed())
			return;

		const auto keyboard = button->getKeyboard();

		keyboard->getOnInput()(getCodeFromCase(keyboard), getNameFromCase(keyboard));

		if (keyboard->getCase() == KeyboardCase::Upper)
			keyboard->setCase(KeyboardCase::Lower);
	}

	// ----------------------------- ActionKeyboardKey -----------------------------

	ShiftKeyboardKey::ShiftKeyboardKey(
		const std::wstring_view& name,
		const std::wstring_view& uppercaseName,
		const std::wstring_view& capsName,
		float width
	) :
		KeyboardKey(
			KeyboardKeyType::Action,
			KeyCode::Shift,
			name,
			width
		),
		_uppercaseName(uppercaseName),
		_capsName(capsName)
	{

	}

	void ShiftKeyboardKey::onPressedChanged(KeyboardButton* button) {
		KeyboardKey::onPressedChanged(button);

		if (button->isPressed())
			return;

		const auto keyboard = button->getKeyboard();

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

	BackspaceKeyboardKey::BackspaceKeyboardKey(const std::wstring_view& name, float width) :
		KeyboardKey(KeyboardKeyType::Action, KeyCode::Backspace, name, width)
	{

	}

	// ----------------------------- EnterKeyboardKey -----------------------------

	EnterKeyboardKey::EnterKeyboardKey(const std::wstring_view& name, float width) :
		KeyboardKey(KeyboardKeyType::Action, KeyCode::Enter, name, width)
	{

	}

	// ----------------------------- SpaceKeyboardKey -----------------------------

	SpaceKeyboardKey::SpaceKeyboardKey() :
		KeyboardKey(KeyboardKeyType::Default, KeyCode::Space, L" ", KeyboardKey::fit)
	{

	}

	// ----------------------------- CharactersLayoutKeyboardKey -----------------------------

	CharactersLayoutKeyboardKey::CharactersLayoutKeyboardKey(
		const std::wstring_view& name,
		float width
	) :
		KeyboardKey(KeyboardKeyType::CharactersLayout, KeyCode::None, name, width)
	{

	}

	void CharactersLayoutKeyboardKey::onPressedChanged(KeyboardButton* button) {
		KeyboardKey::onPressedChanged(button);

		if (button->isPressed())
			return;

		auto keyboard = button->getKeyboard();
		keyboard->setLayout(keyboard->getCharactersLayoutBuilder().value()());
	}

	// ----------------------------- DefaultLayoutKeyboardKey -----------------------------

	CurrentCyclicLayoutKeyboardKey::CurrentCyclicLayoutKeyboardKey(
		const std::wstring_view& name,
		float width
	) :
		KeyboardKey(KeyboardKeyType::Action, KeyCode::None, name, width)
	{

	}

	void CurrentCyclicLayoutKeyboardKey::onPressedChanged(KeyboardButton* button) {
		KeyboardKey::onPressedChanged(button);

		if (button->isPressed())
			return;

		const auto keyboard = button->getKeyboard();

		button->getKeyboard()->setCyclicLayoutIndex(keyboard->getCyclicLayoutIndex());
	}

	// ----------------------------- CyclicLayoutKeyboardKey -----------------------------

	CyclicLayoutKeyboardKey::CyclicLayoutKeyboardKey(
		const std::wstring_view& name,
		float width
	) :
		KeyboardKey(KeyboardKeyType::CyclicLayout, KeyCode::None, name, width)
	{

	}

	void CyclicLayoutKeyboardKey::onPressedChanged(KeyboardButton* button) {
		KeyboardKey::onPressedChanged(button);

		if (!button->isPressed())
			button->getKeyboard()->setNextCyclicLayoutIndex();
	}

	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(Keyboard* keyboard, uint8_t row, uint8_t column) :
		_keyboard(keyboard),
		_row(row),
		_column(column)
	{
		setFocusable(false);
		setCornerRadius(2);
		setFont(_keyboard->getFont());
		updateTextFromCase();

		switch (getKey()->getType()) {
			case KeyboardKeyType::Default: {
				setPrimaryColor(_keyboard->getDefaultButtonPrimaryColor());
				setSecondaryColor(_keyboard->getDefaultButtonSecondaryColor());

				setPressedPrimaryColor(_keyboard->getDefaultButtonSecondaryColor());
				setPressedSecondaryColor(_keyboard->getDefaultButtonPrimaryColor());

				break;
			}
			default: {
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

		getKey()->tick(this);
	}

	void KeyboardButton::onPressedChanged() {
		Button::onPressedChanged();

		getKey()->onPressedChanged(this);
	}

	void KeyboardButton::updateTextFromCase() {
		setText(getKey()->getNameFromCase(getKeyboard()));
	}

	void KeyboardButton::updateFromCase() {
		updateTextFromCase();
	}

	Keyboard* KeyboardButton::getKeyboard() {
		return _keyboard;
	}

	KeyboardKey* KeyboardButton::getKey() {
		return _keyboard->getLayout()->keys[_row][_column];
	}

	uint8_t KeyboardButton::getRow() const {
		return _row;
	}

	uint8_t KeyboardButton::getColumn() const {
		return _column;
	}

	// ----------------------------- Keyboard -----------------------------

	Keyboard::Keyboard() :
		_buttonsContainer(this)
	{
		*this += &_backgroundPanel;
		*this += &_buttonsContainer;
	}

	Keyboard::~Keyboard() {
		deleteLayoutAndUIElements();
	}

	void Keyboard::deleteLayoutAndUIElements() {
		if (!_layout)
			return;

		auto elementsToDelete = std::vector<Element*>(_buttonsContainer.getChildrenCount());

		for (auto buttonElement : _buttonsContainer) {
			elementsToDelete.push_back(buttonElement);
		}

		_buttonsContainer.removeChildren();

		for (auto element : elementsToDelete) {
			delete element;
		}

		delete _layout;

//		Serial.printf("esp_get_free_heap_size: %d\n", esp_get_free_heap_size() / 1024);
//		Serial.printf("esp_get_free_internal_heap_size: %d\n", esp_get_free_internal_heap_size() / 1024);
//		Serial.printf("esp_get_minimum_free_heap_size: %d\n", esp_get_minimum_free_heap_size() / 1024);
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

		for (int rowIndex = 0; rowIndex < _layout->keys.size(); rowIndex++) {
			auto layoutRow = _layout->keys[rowIndex];

			for (int columnIndex = 0; columnIndex < layoutRow.size(); columnIndex++) {
				auto key = layoutRow[columnIndex];

				switch (key->getType()) {
					case KeyboardKeyType::CharactersLayout: {
						if (!_charactersLayoutBuilder.has_value())
							continue;

						break;
					}
					case KeyboardKeyType::CyclicLayout: {
						if (_cyclicLayoutBuilders.size() < 2)
							continue;

						break;
					}
					default:
						break;
				}

				_buttonsContainer += new KeyboardButton(this, rowIndex, columnIndex);
			}
		}
	}

	void Keyboard::setCyclicLayoutIndex(int8_t value) {
		_cyclicLayoutIndex = value;

		setLayout(_cyclicLayoutIndex >= 0 ? _cyclicLayoutBuilders[_cyclicLayoutIndex]() : nullptr);
	}

	int8_t Keyboard::getCyclicLayoutIndex() {
		return _cyclicLayoutIndex;
	}

	uint8_t Keyboard::getHorizontalKeySpacing() const {
		return _horizontalKeySpacing;
	}

	void Keyboard::setHorizontalKeySpacing(uint8_t horizontalKeySpacing) {
		_horizontalKeySpacing = horizontalKeySpacing;
	}

	uint8_t Keyboard::getVerticalKeySpacing() const {
		return _verticalKeySpacing;
	}

	void Keyboard::setVerticalKeySpacing(uint8_t value) {
		_verticalKeySpacing = value;
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

		for (auto element : _buttonsContainer) {
			dynamic_cast<KeyboardButton*>(element)->updateFromCase();
		}
	}

	Callback<KeyCode, bool>& Keyboard::getOnKeyPressedChanged() {
		return _onKeyPressedChanged;
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

	uint8_t Keyboard::getCyclicLayoutsCount() {
		return _cyclicLayoutBuilders.size();
	}

	void Keyboard::setNextCyclicLayoutIndex() {
		auto index = _cyclicLayoutIndex + 1;

		if (index >= getCyclicLayoutsCount())
			index = 0;

		setCyclicLayoutIndex(index);
	}

	std::vector<std::function<KeyboardLayout*()>>& Keyboard::getCyclicLayoutBuilders() {
		return _cyclicLayoutBuilders;
	}

	void Keyboard::setCyclicLayoutBuilders(const std::vector<std::function<KeyboardLayout*()>>& cyclicLayoutBuilders) {
		_cyclicLayoutBuilders = cyclicLayoutBuilders;
	}

	const std::optional<std::function<KeyboardLayout*()>>& Keyboard::getCharactersLayoutBuilder() const {
		return _charactersLayoutBuilder;
	}

	void Keyboard::setCharactersLayoutBuilder(const std::optional<std::function<KeyboardLayout*()>>& charactersLayoutBuilder) {
		_charactersLayoutBuilder = charactersLayoutBuilder;
	}

	// ----------------------------- KeyboardButtonsContainer -----------------------------

	KeyboardButtonsContainer::KeyboardButtonsContainer(Keyboard* keyboard) : _keyboard(keyboard) {

	}

	Size KeyboardButtonsContainer::computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) {
		const auto layout = _keyboard->getLayout();

		if (!layout)
			return { 0, 0 };

		const auto buttonHeight = (uint16_t) (_keyboard->getKeyHeight() * availableSize.getWidth());

		return Size(
			availableSize.getWidth(),
			(buttonHeight + _keyboard->getVerticalKeySpacing()) * layout->keys.size() - _keyboard->getVerticalKeySpacing()
		);
	}

	void KeyboardButtonsContainer::onArrange(const Bounds& bounds) {
		const auto layout = _keyboard->getLayout();

		if (!layout)
			return;

		uint8_t buttonIndexFrom = 0;
		uint8_t rowIndex = 0;
		uint8_t rowButtonCount = 0;
		uint16_t y = bounds.getY();
		const auto buttonHeight = (uint16_t) (_keyboard->getKeyHeight() * bounds.getWidth());

		const auto& arrangeRow = [this, &bounds, &y, &rowButtonCount, &buttonHeight, &buttonIndexFrom](size_t buttonIndexTo) {
			if (rowButtonCount == 0)
				return;

			const uint16_t totalSpacing = _keyboard->getHorizontalKeySpacing() * (rowButtonCount - 1);
			const uint16_t availableWidthWithoutSpacing = bounds.getWidth() - totalSpacing;
			float nonFitWidthWithoutSpacing = 0;
			uint8_t fitCount = 0;
			float buttonWidth;

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				auto button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKey()->getWidth();

				if (buttonWidth == KeyboardKey::fit) {
					fitCount++;
				}
				else {
					nonFitWidthWithoutSpacing += buttonWidth * availableWidthWithoutSpacing;
				}
			}

			float fitWidth;
			float x;

			if (fitCount > 0) {
				fitWidth = (availableWidthWithoutSpacing - nonFitWidthWithoutSpacing) / fitCount;
				x = bounds.getX();
			}
			else {
				const float nonFitWidthWithSpacing = nonFitWidthWithoutSpacing + totalSpacing;

				fitWidth = 0;
				x = bounds.getXCenter() - nonFitWidthWithSpacing / 2;
			}

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				auto button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKey()->getWidth();

				if (buttonWidth == KeyboardKey::fit) {
					buttonWidth = fitWidth;
				}
				else {
					buttonWidth *= availableWidthWithoutSpacing;
				}

				button->arrange(Bounds(
					std::round(x),
					y,
					std::round(buttonWidth),
					buttonHeight
				));

				x += buttonWidth + _keyboard->getHorizontalKeySpacing();
			}
		};

		for (size_t i = 0; i < getChildrenCount(); i++) {
			auto button = dynamic_cast<KeyboardButton*>(getChildAt(i));

			if (button->getRow() > rowIndex) {
				arrangeRow(i);

				y += buttonHeight + _keyboard->getVerticalKeySpacing();
				buttonIndexFrom = i;
				rowButtonCount = 0;
				rowIndex++;
			}

			rowButtonCount++;
		}

		if (buttonIndexFrom < getChildrenCount() - 1)
			arrangeRow(getChildrenCount());
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

	// ----------------------------- KeyboardController -----------------------------

	Keyboard* ApplicationKeyboardController::_keyboard = nullptr;
	Container* ApplicationKeyboardController::_applicationChildrenContainer = nullptr;
	KeyboardApplicationContainer* ApplicationKeyboardController::_keyboardAndApplicationChildrenContainer = nullptr;

	Keyboard* ApplicationKeyboardController::show(Application* application) {
		if (_keyboard)
			return _keyboard;

		_keyboard = new Keyboard();

		_keyboardAndApplicationChildrenContainer = new KeyboardApplicationContainer();
		*_keyboardAndApplicationChildrenContainer += _keyboard;

		_applicationChildrenContainer = new Container();
		_applicationChildrenContainer->setSize(application->getScreenBuffer()->getSize());

		// Moving children from root to temporary layout
		for (auto child : *application)
			*_applicationChildrenContainer += child;

		*_keyboardAndApplicationChildrenContainer += _applicationChildrenContainer;

		application->removeChildren();
		*application += _keyboardAndApplicationChildrenContainer;

		return _keyboard;
	}

	void ApplicationKeyboardController::hide() {
		if (!_keyboard)
			return;

		auto application = _keyboardAndApplicationChildrenContainer->getApplication();

		application->removeChildren();

		// Moving children back to root
		for (auto child : *_applicationChildrenContainer)
			*application += child;

		delete _keyboard;
		_keyboard = nullptr;

		delete _applicationChildrenContainer;
		_applicationChildrenContainer = nullptr;

		delete _keyboardAndApplicationChildrenContainer;
		_keyboardAndApplicationChildrenContainer = nullptr;
	}

	// ----------------------------- NumericKeyboardLayout -----------------------------

	CharactersKeyboardLayout::CharactersKeyboardLayout() : KeyboardLayout({
		{
			&_key1,
			&_key2,
			&_key3,
			&_key4,
			&_key5,
			&_key6,
			&_key7,
			&_key8,
			&_key9,
			&_key0
		},
		{
			&_keyAt,
			&_keyNumberSign,
			&_keyDollar,
			&_keyUnderscore,
			&_keyAmpersand,
			&_keyMinus,
			&_keyPlus,
			&_keyLeftBrace,
			&_keyRightBrace,
			&_keySlash,
		},
		{
			&_keyShift,
			&_keyAsterisk,
			&_keyDoubleQuote,
			&_keyQuote,
			&_keyColon,
			&_keySemicolon,
			&_keyExclamationMark,
			&_keyQuestionMark,
			&_keyBackspace,
		},
		{
			&_keyCyclicLayout,
			&_keyComma,
			&_keySpace,
			&_keyPeriod,
			&_keyEnter,
		}
	}) {

	}

	// ----------------------------- EnglishKeyboardLayout -----------------------------

	EnglishKeyboardLayout::EnglishKeyboardLayout() : KeyboardLayout({
		{
			&_keyQ,
			&_keyW,
			&_keyE,
			&_keyR,
			&_keyT,
			&_keyY,
			&_keyU,
			&_keyI,
			&_keyO,
			&_keyP,
		},
		{
			&_keyA,
			&_keyS,
			&_keyD,
			&_keyF,
			&_keyG,
			&_keyH,
			&_keyJ,
			&_keyK,
			&_keyL
		},
		{
			&_keyShift,
			&_keyZ,
			&_keyX,
			&_keyC,
			&_keyV,
			&_keyB,
			&_keyN,
			&_keyM,
			&_keyBackspace,
		},
		{
			&_keyCharactersLayout,
			&_keyCyclicLayout,
			&_keyComma,
			&_keySpace,
			&_keyPeriod,
			&_keyEnter,
		}
	}) {

	}

	// ----------------------------- RussianKeyboardLayout -----------------------------

	RussianKeyboardLayout::RussianKeyboardLayout() : KeyboardLayout({
		{
			&_keyQ,
			&_keyW,
			&_keyE,
			&_keyR,
			&_keyT,
			&_keyY,
			&_keyU,
			&_keyI,
			&_keyO,
			&_keyP,
			&_keyBracket
		},
		{
			&_keyA,
			&_keyS,
			&_keyD,
			&_keyF,
			&_keyG,
			&_keyH,
			&_keyJ,
			&_keyK,
			&_keyL,
			&_keySemicolon,
			&_keyQuote
		},
		{
			&_keyShift,
			&_keyZ,
			&_keyX,
			&_keyC,
			&_keyV,
			&_keyB,
			&_keyN,
			&_keyM,
			&_keyLessThan,
			&_keyMoreThan,
			&_keyBackspace,
		},
		{
			&_keyCharactersLayout,
			&_keyCyclicLayout,
			&_keyComma,
			&_keySpace,
			&_keyPeriod,
			&_keyEnter,
		}
	}) {

	}
}