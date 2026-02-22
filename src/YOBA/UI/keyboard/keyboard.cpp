#include <YOBA/UI/keyboard/keyboard.h>
#include <YOBA/UI/keyboard/layout.h>

namespace YOBA {
	// ----------------------------- Keyboard -----------------------------

	Keyboard::Keyboard() {
		*this += _backgroundPanel;
	}

	Keyboard::~Keyboard() {
		deleteLayoutAndUIElements();
	}

	void Keyboard::deleteLayoutAndUIElements() {
		if (!_layout)
			return;

		this->removeChild(*_layout);

		delete _layout;
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
		deleteLayoutAndUIElements();

		_case = KeyboardCase::lower;
		_layout = value;

		if (!_layout)
			return;

		*this += *_layout;
		_layout->assignKeyboard(this);
	}

	uint8_t Keyboard::getLayoutOptions() const {
		return _layoutOptions;
	}

	uint8_t Keyboard::getCyclicLayoutIndex() const {
		return _cyclicLayoutIndex;
	}

	void Keyboard::setCyclicLayoutIndex(const uint8_t value) {
		_cyclicLayoutIndex = value;

		setLayout(_cyclicLayoutBuilders[_cyclicLayoutIndex]());
	}

	uint8_t Keyboard::getHorizontalKeySpacing() const {
		return _horizontalKeySpacing;
	}

	void Keyboard::setHorizontalKeySpacing(const uint8_t horizontalKeySpacing) {
		_horizontalKeySpacing = horizontalKeySpacing;
	}

	uint8_t Keyboard::getVerticalKeySpacing() const {
		return _verticalKeySpacing;
	}

	void Keyboard::setVerticalKeySpacing(const uint8_t value) {
		_verticalKeySpacing = value;
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
			const auto& layoutRow =  _layout->getButtons()[rowIndex];

			for (size_t columnIndex = 0; columnIndex < layoutRow.size(); columnIndex++) {
				const auto button = layoutRow[columnIndex];

				button->updateFromCase();
			}
		}
	}

	uint16_t Keyboard::getContinuousTypingDelay() const {
		return _continuousTypingDelay;
	}

	void Keyboard::setContinuousTypingDelay(const uint16_t value) {
		_continuousTypingDelay = value;
	}

	uint16_t Keyboard::getContinuousTypingInterval() const {
		return _continuousTypingInterval;
	}

	void Keyboard::setContinuousTypingInterval(const uint16_t value) {
		_continuousTypingInterval = value;
	}

	uint8_t Keyboard::getCyclicLayoutsCount() const {
		return _cyclicLayoutBuilders.size();
	}

	void Keyboard::setNextCyclicLayoutIndex() {
		auto index = _cyclicLayoutIndex + 1;

		if (index >= getCyclicLayoutsCount())
			index = 0;

		setCyclicLayoutIndex(index);
	}

	void Keyboard::setLayout(const uint8_t options) {
		if (options == _layoutOptions)
			return;

		_layoutOptions = options;

		if (_layoutOptions & KeyboardLayoutOptions::text) {
			setCyclicLayoutIndex(getCyclicLayoutIndex());
		}
		else if (_layoutOptions & KeyboardLayoutOptions::numeric) {
			if (getNumericLayoutBuilder()) {
				setLayout(getNumericLayoutBuilder()());
			}
			else {
				setCyclicLayoutIndex(getCyclicLayoutIndex());
			}
		}
	}

	std::vector<std::function<KeyboardLayout*()>>& Keyboard::getCyclicLayoutBuilders() {
		return _cyclicLayoutBuilders;
	}

	void Keyboard::setCyclicLayoutBuilders(const std::vector<std::function<KeyboardLayout*()>>& builder) {
		_cyclicLayoutBuilders = builder;
	}

	const std::function<KeyboardLayout*()>& Keyboard::getCharactersLayoutBuilder() const {
		return _charactersLayoutBuilder;
	}

	void Keyboard::setCharactersLayoutBuilder(const std::function<KeyboardLayout*()>& builder) {
		_charactersLayoutBuilder = builder;
	}
	
	const std::function<KeyboardLayout*()>& Keyboard::getNumericLayoutBuilder() const {
		return _numericLayoutBuilder;
	}

	void Keyboard::setNumericLayoutBuilder(const std::function<KeyboardLayout*()>& builder) {
		_numericLayoutBuilder = builder;
	}

	uint8_t Keyboard::getKeyCornerRadius() const {
		return _keyCornerRadius;
	}

	void Keyboard::setKeyCornerRadius(const uint8_t value) {
		_keyCornerRadius = value;
	}
}