#include <cstdint>

#include <YOBA/main/math.h>
#include <YOBA/UI/keyboard/layout.h>
#include <YOBA/UI/keyboard/keyboard.h>
#include <YOBA/UI/application.h>

namespace YOBA {
	KeyboardLayout::KeyboardLayout(const std::vector<std::vector<KeyboardButton*>>& keys) : _buttons(keys) {

	}

	const std::vector<std::vector<KeyboardButton*>>& KeyboardLayout::getButtons() const {
		return _buttons;
	}

	void KeyboardLayout::assignKeyboard(Keyboard* keyboard) {
		_keyboard = keyboard;

		removeChildren();

		for (size_t rowIndex = 0; rowIndex < _buttons.size(); rowIndex++) {
			const auto& layoutRow = _buttons[rowIndex];

			for (size_t columnIndex = 0; columnIndex < layoutRow.size(); columnIndex++) {
				auto& button = *layoutRow[columnIndex];

				button.assignKeyboard(_keyboard, rowIndex, columnIndex);

				if (!button.canBeAdded())
					continue;

				*this += button;
			}
		}
	}

	Size KeyboardLayout::onMeasure(const Size& availableSize) {
		const auto layout = _keyboard->getLayout();

		return
			layout
			? Size(
				availableSize.getWidth(),
				(_keyboard->getKeyHeight() + _keyboard->getVerticalKeySpacing()) * layout->getButtons().size() - _keyboard->getVerticalKeySpacing()
			)
			: Size();
	}

	void KeyboardLayout::onRender(Renderer& renderer, const Bounds& bounds) {
		const auto layout = _keyboard->getLayout();

		if (!layout)
			return;

		uint8_t buttonIndexFrom = 0;
		uint8_t rowIndex = 0;
		uint8_t rowButtonCount = 0;
		uint16_t y = bounds.getY();

		const auto& arrangeRow = [this, &bounds, &y, &rowButtonCount, &buttonIndexFrom, &renderer](const size_t buttonIndexTo) {
			if (rowButtonCount == 0)
				return;

			const uint16_t totalSpacing = _keyboard->getHorizontalKeySpacing() * (rowButtonCount - 1);
			const uint16_t availableWidthWithoutSpacing = bounds.getWidth() - totalSpacing;
			float defaultWidthWithoutSpacing = 0;
			uint8_t stretchedCount = 0;
			float buttonWidth;

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				auto& button = dynamic_cast<KeyboardButton&>(getChildAt(i));

				buttonWidth = button.getKeyWidth();

				if (buttonWidth == KeyboardButton::stretched) {
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
				auto& button = dynamic_cast<KeyboardButton&>(getChildAt(i));

				buttonWidth = button.getKeyWidth();

				buttonWidth =
					// Not last key in row
					i < buttonIndexTo - 1
					? (
						buttonWidth == KeyboardButton::stretched
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

				button.render(renderer, Bounds(
					bounds.getX() + localX,
					y,
					static_cast<uint16_t>(buttonWidth),
					_keyboard->getKeyHeight()
				));

				localX += static_cast<uint16_t>(buttonWidth) + _keyboard->getHorizontalKeySpacing();
			}
		};

		for (size_t i = 0; i < getChildrenCount(); i++) {
			const auto& button = dynamic_cast<KeyboardButton&>(getChildAt(i));

			if (button.getRow() > rowIndex) {
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

	// ----------------------------- NumericKeyboardLayout -----------------------------

	NumericKeyboardLayout::NumericKeyboardLayout() : KeyboardLayout({
		{
			&_key1,
			&_key2,
			&_key3,
			&_keyDummy30
		},
		{
			&_key4,
			&_key5,
			&_key6,
			&_keyDummy31
		},
		{
			&_key7,
			&_key8,
			&_key9,
			&_keyBackspace
		},
		{
			&_keyMinus,
			&_key0,
			&_keyPeriod,
			&_keyEnter
		},
	}) {

	}

	// ----------------------------- CharactersKeyboardLayout -----------------------------


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