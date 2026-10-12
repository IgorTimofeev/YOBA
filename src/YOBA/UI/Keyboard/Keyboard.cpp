#include <YOBA/UI/Keyboard/Keyboard.hpp>
#include <YOBA/UI/Keyboard/Layout.hpp>

namespace YOBA {
	// -------------------------------- KeyboardLayoutController --------------------------------

	size_t KeyboardLayoutController::getCyclicLayoutCount() {
		return 1;
	}

	KeyboardLayout* KeyboardLayoutController::getCyclicLayout(size_t index) {
		return new EnglishKeyboardLayout();
	}

	std::optional<KeyboardLayout*> KeyboardLayoutController::getCharactersLayout() {
		return new CharactersKeyboardLayout();
	}

	std::optional<KeyboardLayout*> KeyboardLayoutController::getNumericLayout() {
		return new NumericKeyboardLayout();
	}

	void KeyboardLayoutController::onLayoutChanged(KeyboardLayout* fromLayout, KeyboardLayout* toLayout) {
		if (fromLayout)
			delete fromLayout;
	}

	// -------------------------------- Keyboard --------------------------------


	Keyboard::Keyboard() {
		*this += &_backgroundPanel;
	}

	Keyboard::~Keyboard() {
		setLayout(nullptr);
	}

	const Color* Keyboard::getBackgroundColor() const {
		return _backgroundPanel.getFillColor();
	}

	void Keyboard::setBackgroundColor(const Color* value) {
		_backgroundPanel.setFillColor(value);
	}

	const Color* Keyboard::getDefaultKeyBackgroundColor() const {
		return _defaultKeyBackgroundColor;
	}

	void Keyboard::setDefaultKeyBackgroundColor(const Color* value) {
		_defaultKeyBackgroundColor = value;
	}

	const Color* Keyboard::getDefaultKeyTextColor() const {
		return _defaultKeyTextColor;
	}

	void Keyboard::setDefaultKeyTextColor(const Color* value) {
		_defaultKeyTextColor = value;
	}

	const Color* Keyboard::getActionKeyBackgroundColor() const {
		return _actionKeyBackgroundColor;
	}

	void Keyboard::setActionKeyBackgroundColor(const Color* value) {
		_actionKeyBackgroundColor = value;
	}

	const Color* Keyboard::getDisabledKeyBackgroundColor() const {
		return _disabledKeyBackgroundColor;
	}

	void Keyboard::setDisabledKeyBackgroundColor(const Color* value) {
		_disabledKeyBackgroundColor = value;
	}

	const Color* Keyboard::getDisabledKeyTextColor() const {
		return _disabledKeyTextColor;
	}

	void Keyboard::setDisabledKeyTextColor(const Color* value) {
		_disabledKeyTextColor = value;
	}

	const Color* Keyboard::getActionKeyTextColor() const {
		return _actionKeyTextColor;
	}

	void Keyboard::setActionKeyTextColor(const Color* actionButtonSecondaryColor) {
		_actionKeyTextColor = actionButtonSecondaryColor;
	}

	KeyboardLayout* Keyboard::getLayout() const {
		return _layout;
	}

	void Keyboard::setLayout(KeyboardLayout* value) {
		// Removing old layout
		const auto oldLayout = _layout;

		this->removeChild(_layout);

		// Setting new layout
		_case = KeyboardCase::lower;
		_layout = value;

		if (_layout) {
			*this += _layout;
			_layout->setKeyboard(this);
		}

		// Handling change
		_layoutController->onLayoutChanged(oldLayout, _layout);
	}

	uint8_t Keyboard::getTextParsingOptions() const {
		return _textParsingOptions;
	}

	uint8_t Keyboard::getCyclicLayoutIndex() const {
		return _cyclicLayoutIndex;
	}

	void Keyboard::setLayoutFromCyclicIndex(const uint8_t cyclicLayoutIndex) {
		_cyclicLayoutIndex = cyclicLayoutIndex;

		setLayout(_layoutController->getCyclicLayout(_cyclicLayoutIndex));
	}

	uint8_t Keyboard::getHorizontalKeyGap() const {
		return _horizontalKeyGap;
	}

	void Keyboard::setHorizontalKeyGap(const uint8_t value) {
		_horizontalKeyGap = value;
	}

	uint8_t Keyboard::getVerticalKeyGap() const {
		return _verticalKeyGap;
	}

	void Keyboard::setVerticalKeyGap(const uint8_t value) {
		_verticalKeyGap = value;
	}

	uint16_t Keyboard::getKeyHeight() const {
		return _keyHeight;
	}

	void Keyboard::setKeyHeight(const uint16_t value) {
		_keyHeight = value;
	}

	KeyboardCase Keyboard::getCase() const {
		return _case;
	}

	void Keyboard::setCase(const KeyboardCase value) {
		_case = value;

		if (!_layout)
			return;

		for (size_t rowIndex = 0; rowIndex < _layout->getButtons().size(); rowIndex++) {
			const auto& layoutRow = _layout->getButtons()[rowIndex];

			for (size_t columnIndex = 0; columnIndex < layoutRow.size(); columnIndex++) {
				const auto button = layoutRow[columnIndex];

				button->updateFromCase();
			}
		}
	}

	KeyboardLayoutController* Keyboard::getLayoutController() const {
		return _layoutController;
	}

	void Keyboard::setLayoutController(KeyboardLayoutController* layoutController) {
		_layoutController = layoutController;
	}

	uint32_t Keyboard::getContinuousTypingDelay() const {
		return _continuousTypingDelay;
	}

	void Keyboard::setContinuousTypingDelay(const uint32_t value) {
		_continuousTypingDelay = value;
	}

	uint32_t Keyboard::getContinuousTypingInterval() const {
		return _continuousTypingInterval;
	}

	void Keyboard::setContinuousTypingInterval(const uint32_t value) {
		_continuousTypingInterval = value;
	}

	void Keyboard::setNextCyclicLayoutIndex() {
		auto index = _cyclicLayoutIndex + 1;

		if (index >= _layoutController->getCyclicLayoutCount())
			index = 0;

		setLayoutFromCyclicIndex(index);
	}

	void Keyboard::setLayoutFromTextParsingOptions(const uint8_t options) {
		if (options == _textParsingOptions && _textParsingOptionsSet)
			return;

		_textParsingOptions = options;
		_textParsingOptionsSet = true;

		if (_textParsingOptions == TextParsingOptions::none) {
			setLayoutFromCyclicIndex(_cyclicLayoutIndex);
		}
		else if (_textParsingOptions & TextParsingOptions::number) {
			if (_layoutController->getNumericLayout().has_value()) {
				setLayout(_layoutController->getNumericLayout().value());
			}
			else {
				setLayoutFromCyclicIndex(_cyclicLayoutIndex);
			}
		}
	}

	uint8_t Keyboard::getKeyCornerRadius() const {
		return _keyCornerRadius;
	}

	void Keyboard::setKeyCornerRadius(const uint8_t value) {
		_keyCornerRadius = value;
	}
}