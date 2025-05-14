#include <cstdint>
#include "keyboard.h"
#include "stackLayout.h"
#include "application.h"
#include "YOBA/main/math.h"

namespace YOBA {
	// ----------------------------- KeyboardKey -----------------------------

	KeyboardKey::KeyboardKey(KeyboardKeyType type, KeyCode code, std::wstring_view name, float width) :
		_type(type),
		_code(code),
		_name(name),
		_width(width)
	{

	}

	float KeyboardKey::stretched = -1;

	void KeyboardKey::tick(KeyboardButton* button) {

	}

	void KeyboardKey::onKeyPressedChanged(KeyboardButton* button) {
		const auto keyboard = button->getKeyboard();
		const auto code = getCodeFromCase(keyboard);

		if (code != KeyCode::none)
			keyboard->keyPressedChanged(code, button->isActive());
	}

	KeyCode KeyboardKey::getCode() const {
		return _code;
	}

	std::wstring_view KeyboardKey::getName() const {
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

	std::wstring_view KeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		return getName();
	}

	// ----------------------------- TextKeyboardKey -----------------------------

	TextKeyboardKey::TextKeyboardKey(
		KeyCode code,
		std::wstring_view name,
		KeyCode uppercaseCode,
		std::wstring_view uppercaseName,
		float width
	) :
		KeyboardKey(KeyboardKeyType::normal, code, name, width),
		_uppercaseCode(uppercaseCode),
		_uppercaseName(uppercaseName)
	{

	}

	TextKeyboardKey::TextKeyboardKey(KeyCode code, std::wstring_view name, float width) :
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

	std::wstring_view TextKeyboardKey::getUppercaseName() const {
		return _uppercaseName;
	}

	KeyCode TextKeyboardKey::getCodeFromCase(Keyboard* keyboard) const {
		return keyboard->getCase() == KeyboardCase::lower ? getCode() : getUppercaseCode();
	}

	std::wstring_view TextKeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		return keyboard->getCase() == KeyboardCase::lower ? getName() : getUppercaseName();
	}

	void TextKeyboardKey::onKeyPressedChanged(KeyboardButton* button) {
		KeyboardKey::onKeyPressedChanged(button);

		if (button->isActive())
			return;

		const auto keyboard = button->getKeyboard();

		keyboard->input(getCodeFromCase(keyboard), getNameFromCase(keyboard));

		if (keyboard->getCase() == KeyboardCase::upper)
			keyboard->setCase(KeyboardCase::lower);
	}

	// ----------------------------- ActionKeyboardKey -----------------------------

	ShiftKeyboardKey::ShiftKeyboardKey(
		std::wstring_view name,
		std::wstring_view uppercaseName,
		std::wstring_view capsName,
		float width
	) :
		KeyboardKey(
			KeyboardKeyType::action,
			KeyCode::shift,
			name,
			width
		),
		_uppercaseName(uppercaseName),
		_capsName(capsName)
	{

	}

	void ShiftKeyboardKey::onKeyPressedChanged(KeyboardButton* button) {
		KeyboardKey::onKeyPressedChanged(button);

		if (button->isActive())
			return;

		const auto keyboard = button->getKeyboard();

		switch (keyboard->getCase()) {
			case KeyboardCase::lower:
				keyboard->setCase(KeyboardCase::upper);
				break;
			case KeyboardCase::upper:
				keyboard->setCase(KeyboardCase::caps);
				break;
			default:
				keyboard->setCase(KeyboardCase::lower);
				break;
		}
	}

	std::wstring_view ShiftKeyboardKey::getNameFromCase(Keyboard* keyboard) const {
		switch (keyboard->getCase()) {
			case KeyboardCase::lower:
				return getName();
			case KeyboardCase::upper:
				return _uppercaseName;
			default:
				return _capsName;
		}
	}

	// ----------------------------- BackspaceKeyboardKey -----------------------------

	BackspaceKeyboardKey::BackspaceKeyboardKey(std::wstring_view name, float width) :
		KeyboardKey(KeyboardKeyType::action, KeyCode::backspace, name, width)
	{

	}

	// ----------------------------- EnterKeyboardKey -----------------------------

	EnterKeyboardKey::EnterKeyboardKey(float width) :
		KeyboardKey(KeyboardKeyType::action, KeyCode::enter, L"<-", width)
	{

	}

	// ----------------------------- SpaceKeyboardKey -----------------------------

	SpaceKeyboardKey::SpaceKeyboardKey() :
		TextKeyboardKey(KeyCode::space, L" ", KeyCode::space, L" ", stretched)
	{

	}

	// ----------------------------- CharactersLayoutKeyboardKey -----------------------------

	CharactersLayoutKeyboardKey::CharactersLayoutKeyboardKey(
		std::wstring_view name,
		float width
	) :
		KeyboardKey(KeyboardKeyType::charactersLayout, KeyCode::none, name, width)
	{

	}

	void CharactersLayoutKeyboardKey::onKeyPressedChanged(KeyboardButton* button) {
		KeyboardKey::onKeyPressedChanged(button);

		if (button->isActive())
			return;

		const auto keyboard = button->getKeyboard();

		keyboard->setLayout(keyboard->getCharactersLayoutBuilder().value()());
	}

	// ----------------------------- DefaultLayoutKeyboardKey -----------------------------

	CurrentCyclicLayoutKeyboardKey::CurrentCyclicLayoutKeyboardKey(
		std::wstring_view name,
		float width
	) :
		KeyboardKey(KeyboardKeyType::action, KeyCode::none, name, width)
	{

	}

	void CurrentCyclicLayoutKeyboardKey::onKeyPressedChanged(KeyboardButton* button) {
		KeyboardKey::onKeyPressedChanged(button);

		if (button->isActive())
			return;

		const auto keyboard = button->getKeyboard();

		keyboard->setCyclicLayoutIndex(keyboard->getCyclicLayoutIndex());
	}

	// ----------------------------- CyclicLayoutKeyboardKey -----------------------------

	CyclicLayoutKeyboardKey::CyclicLayoutKeyboardKey(
		std::wstring_view name,
		float width
	) :
		KeyboardKey(KeyboardKeyType::cyclicLayout, KeyCode::none, name, width)
	{

	}

	void CyclicLayoutKeyboardKey::onKeyPressedChanged(KeyboardButton* button) {
		KeyboardKey::onKeyPressedChanged(button);

		if (!button->isActive()) {
			button->getKeyboard()->setNextCyclicLayoutIndex();
		}
	}

	// ----------------------------- KeyboardButton -----------------------------

	KeyboardButton::KeyboardButton(Keyboard* keyboard, uint8_t row, uint8_t column) :
		_keyboard(keyboard),
		_row(row),
		_column(column)
	{
		setFocusable(false);
		setCornerRadius(_keyboard->getKeyCornerRadius());
		setFont(_keyboard->getFont());
		updateTextFromCase();

		switch (getKey()->getType()) {
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

	void KeyboardButton::onTick() {
		Element::onTick();

		getKey()->tick(this);
	}

	void KeyboardButton::onIsActiveChanged() {
		Button::onIsActiveChanged();

		Application::getCurrent()->scheduleTask([this] {
			getKey()->onKeyPressedChanged(this);
		});
	}

	void KeyboardButton::updateTextFromCase() {
		setText(getKey()->getNameFromCase(getKeyboard()));
	}

	void KeyboardButton::updateFromCase() {
		updateTextFromCase();
	}

	Keyboard* KeyboardButton::getKeyboard() const {
		return _keyboard;
	}

	KeyboardKey* KeyboardButton::getKey() const {
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
		_buttonsLayout(this)
	{
		*this += &_backgroundPanel;
		*this += &_buttonsLayout;
	}

	Keyboard::~Keyboard() {
		deleteLayoutAndUIElements();
	}

	void Keyboard::deleteLayoutAndUIElements() {
		if (!_layout)
			return;

		auto elementsToDelete = std::vector<Element*>(_buttonsLayout.getChildrenCount());

		for (auto buttonElement : _buttonsLayout) {
			elementsToDelete.push_back(buttonElement);
		}

		_buttonsLayout.removeChildren();

		for (const auto element : elementsToDelete) {
			delete element;
		}

		delete _layout;
	}

	const Color* Keyboard::getBackgroundColor() const {
		return _backgroundPanel.getFillColor();
	}

	void Keyboard::setBackgroundColor(const Color* value) {
		_backgroundPanel.setFillColor(value);
	}

	const Color* Keyboard::getDefaultKeyBackgroundColor() const {
		return _defaultKeyPrimaryColor;
	}

	void Keyboard::setDefaultKeyBackgroundColor(const Color* value) {
		_defaultKeyPrimaryColor = value;
	}

	const Color* Keyboard::getDefaultKeyTextColor() const {
		return _defaultKeySecondaryColor;
	}

	void Keyboard::setDefaultKeyTextColor(const Color* value) {
		_defaultKeySecondaryColor = value;
	}

	const Color* Keyboard::getActionKeyBackgroundColor() const {
		return _actionKeyPrimaryColor;
	}

	void Keyboard::setActionKeyBackgroundColor(const Color* value) {
		_actionKeyPrimaryColor = value;
	}

	const Color* Keyboard::getActionKeyTextColor() const {
		return _actionKeySecondaryColor;
	}

	void Keyboard::setActionKeySecondaryColor(const Color* actionButtonSecondaryColor) {
		_actionKeySecondaryColor = actionButtonSecondaryColor;
	}

	KeyboardLayout* Keyboard::getLayout() const {
		return _layout;
	}

	void Keyboard::setLayout(KeyboardLayout* value) {
		deleteLayoutAndUIElements();

		_case = KeyboardCase::lower;
		_layout = value;

		if (!_layout)
			return;

		for (int rowIndex = 0; rowIndex < _layout->keys.size(); rowIndex++) {
			auto layoutRow = _layout->keys[rowIndex];

			for (int columnIndex = 0; columnIndex < layoutRow.size(); columnIndex++) {
				const auto key = layoutRow[columnIndex];

				switch (key->getType()) {
					case KeyboardKeyType::charactersLayout: {
						if (!_charactersLayoutBuilder.has_value())
							continue;

						break;
					}
					case KeyboardKeyType::cyclicLayout: {
						if (_cyclicLayoutBuilders.size() < 2)
							continue;

						break;
					}
					default:
						break;
				}

				_buttonsLayout += new KeyboardButton(this, rowIndex, columnIndex);
			}
		}
	}

	void Keyboard::setCyclicLayoutIndex(int8_t value) {
		_cyclicLayoutIndex = value;

		setLayout(_cyclicLayoutIndex >= 0 ? _cyclicLayoutBuilders[_cyclicLayoutIndex]() : nullptr);
	}

	int8_t Keyboard::getCyclicLayoutIndex() const {
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

	uint16_t Keyboard::getKeyHeight() const {
		return _keyHeight;
	}

	void Keyboard::setKeyHeight(uint16_t value) {
		_keyHeight = value;
	}

	KeyboardCase Keyboard::getCase() const {
		return _case;
	}

	void Keyboard::setCase(KeyboardCase value) {
		_case = value;

		if (!_layout)
			return;

		for (const auto element : _buttonsLayout) {
			dynamic_cast<KeyboardButton*>(element)->updateFromCase();
		}
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

	uint8_t Keyboard::getCyclicLayoutsCount() const {
		return _cyclicLayoutBuilders.size();
	}

	void Keyboard::setNextCyclicLayoutIndex() {
		auto index = static_cast<int8_t>(_cyclicLayoutIndex + 1);

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

	uint8_t Keyboard::getKeyCornerRadius() const {
		return _keyCornerRadius;
	}

	void Keyboard::setKeyCornerRadius(uint8_t value) {
		_keyCornerRadius = value;
	}

	// ----------------------------- KeyboardButtonsLayout -----------------------------

	KeyboardButtonsLayout::KeyboardButtonsLayout(Keyboard* keyboard) : _keyboard(keyboard) {

	}

	Size KeyboardButtonsLayout::onMeasure(const Size& availableSize) {
		const auto layout = _keyboard->getLayout();

		return
			layout
			? Size(
				availableSize.getWidth(),
				(_keyboard->getKeyHeight() + _keyboard->getVerticalKeySpacing()) * layout->keys.size() - _keyboard->getVerticalKeySpacing()
			)
			: Size();
	}

	void KeyboardButtonsLayout::onRender(Renderer* renderer) {
		const auto layout = _keyboard->getLayout();

		if (!layout)
			return;

		const auto& bounds = getBounds();

		uint8_t buttonIndexFrom = 0;
		uint8_t rowIndex = 0;
		uint8_t rowButtonCount = 0;
		uint16_t y = bounds.getY();

		const auto& arrangeRow = [this, &bounds, &y, &rowButtonCount, &buttonIndexFrom, &renderer](size_t buttonIndexTo) {
			if (rowButtonCount == 0)
				return;

			const uint16_t totalSpacing = _keyboard->getHorizontalKeySpacing() * (rowButtonCount - 1);
			const uint16_t availableWidthWithoutSpacing = bounds.getWidth() - totalSpacing;
			float defaultWidthWithoutSpacing = 0;
			uint8_t stretchedCount = 0;
			KeyboardButton* button;
			float buttonWidth;

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKey()->getWidth();

				if (buttonWidth == KeyboardKey::stretched) {
					stretchedCount++;
				}
				else {
					defaultWidthWithoutSpacing += buttonWidth * static_cast<float>(availableWidthWithoutSpacing);
				}
			}

			float stretchedWidth;
			int32_t localX;

			const float defaultWidthWithSpacing = defaultWidthWithoutSpacing + static_cast<float>(totalSpacing);

			// 1 is for float rounding correction
			const auto rowIsFullOfDefaultKeys = defaultWidthWithSpacing >= static_cast<float>(bounds.getWidth() - 1);

			if (stretchedCount > 0) {
				stretchedWidth = (static_cast<float>(availableWidthWithoutSpacing) - defaultWidthWithoutSpacing) / static_cast<float>(stretchedCount);
				localX = bounds.getX();
			}
			else {
				stretchedWidth = 0;
				localX = bounds.getXCenter() - static_cast<int32_t>(defaultWidthWithSpacing / 2);
			}

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKey()->getWidth();

				buttonWidth =
					// Not last key in row
					i < buttonIndexTo - 1
					? (
						buttonWidth == KeyboardKey::stretched
						? std::round(stretchedWidth)
						: std::round(buttonWidth * static_cast<float>(availableWidthWithoutSpacing))
					)
					// Last
					: (
						stretchedCount > 0 || rowIsFullOfDefaultKeys
						// Stretching to end
						? static_cast<float>(bounds.getWidth() - localX)
						// Using desired key size
						: std::round(buttonWidth * static_cast<float>(availableWidthWithoutSpacing))
					);

				button->render(renderer, Bounds(
					bounds.getX() + localX,
					y,
					static_cast<uint16_t>(buttonWidth),
					_keyboard->getKeyHeight()
				));

				localX += static_cast<uint16_t>(buttonWidth) + _keyboard->getHorizontalKeySpacing();
			}
		};

		for (size_t i = 0; i < getChildrenCount(); i++) {
			const auto button = dynamic_cast<KeyboardButton*>(getChildAt(i));

			if (button->getRow() > rowIndex) {
				arrangeRow(i);

				y += _keyboard->getKeyHeight() + _keyboard->getVerticalKeySpacing();
				buttonIndexFrom = i;
				rowButtonCount = 0;
				rowIndex++;
			}

			rowButtonCount++;
		}

		if (buttonIndexFrom < getChildrenCount() - 1)
			arrangeRow(getChildrenCount());
	}

	// ----------------------------- KeyboardController -----------------------------

	Keyboard* ApplicationKeyboardController::_keyboard = nullptr;
	Layout* ApplicationKeyboardController::_applicationChildrenLayout = nullptr;
	RelativeStackLayout* ApplicationKeyboardController::_keyboardAndApplicationChildrenLayout = nullptr;

	Keyboard* ApplicationKeyboardController::show() {
		if (_keyboard)
			return nullptr;

		const auto application = Application::getCurrent();

		_keyboardAndApplicationChildrenLayout = new RelativeStackLayout();
		_keyboardAndApplicationChildrenLayout->setSize(application->getRenderer()->getTarget()->getSize());

		// Moving children from root to temporary layout
		_applicationChildrenLayout = new Layout();

		for (const auto child : *application)
			*_applicationChildrenLayout += child;

		application->removeChildren();

		*_keyboardAndApplicationChildrenLayout += _applicationChildrenLayout;

		// Creating keyboard
		_keyboard = new Keyboard();
		_keyboardAndApplicationChildrenLayout->setAutoSize(_keyboard);
		*_keyboardAndApplicationChildrenLayout += _keyboard;

		*application += _keyboardAndApplicationChildrenLayout;

		return _keyboard;
	}

	void ApplicationKeyboardController::hide() {
		if (!_keyboard)
			return;

		const auto application = Application::getCurrent();

		application->removeChildren();

		// Moving children back to root
		for (const auto child : *_applicationChildrenLayout)
			*application += child;

		delete _keyboard;
		_keyboard = nullptr;

		delete _applicationChildrenLayout;
		_applicationChildrenLayout = nullptr;

		delete _keyboardAndApplicationChildrenLayout;
		_keyboardAndApplicationChildrenLayout = nullptr;
	}

	bool ApplicationKeyboardController::isVisible() {
		return _keyboard != nullptr;
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