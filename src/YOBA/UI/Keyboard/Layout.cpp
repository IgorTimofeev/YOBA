#include <cstdint>

#include <YOBA/Core/Math.hpp>
#include <YOBA/UI/Keyboard/Layout.hpp>
#include <YOBA/UI/Keyboard/Keyboard.hpp>
#include <YOBA/UI/Application.hpp>

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
				const auto button = layoutRow[columnIndex];

				button->assignKeyboard(_keyboard, rowIndex, columnIndex);

				if (!button->canBeAdded())
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
				(_keyboard->getKeyHeight() + _keyboard->getVerticalKeyGap()) * layout->getButtons().size() - _keyboard->getVerticalKeyGap()
			)
			: Size();
	}

	void KeyboardLayout::onArrange(const Rectangle& bounds) {
		const auto layout = _keyboard->getLayout();

		if (!layout)
			return;

		uint8_t buttonIndexFrom = 0;
		uint8_t rowIndex = 0;
		uint8_t rowButtonCount = 0;
		uint16_t y = bounds.getY();

		const auto& arrangeRow = [this, &bounds, &y, &rowButtonCount, &buttonIndexFrom](const size_t buttonIndexTo) {
			if (rowButtonCount == 0)
				return;

			const uint16_t totalGap = _keyboard->getHorizontalKeyGap() * (rowButtonCount - 1);
			const uint16_t availableWidthWithoutGap = bounds.getWidth() - totalGap;
			float defaultWidthWithoutGap = 0;
			uint8_t stretchedCount = 0;
			KeyboardButton* button;
			float buttonWidth;

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKeyWidth();

				if (buttonWidth == KeyboardButton::stretched) {
					stretchedCount++;
				}
				else {
					defaultWidthWithoutGap += buttonWidth * static_cast<float>(availableWidthWithoutGap);
				}
			}

			float stretchedWidth;
			int32_t localX;

			const float defaultWidthWithGap = defaultWidthWithoutGap + static_cast<float>(totalGap);

			// 1 is for float rounding correction
			const auto rowIsFullOfDefaultKeys = defaultWidthWithGap >= static_cast<float>(bounds.getWidth() - 1);

			if (stretchedCount > 0) {
				stretchedWidth = (static_cast<float>(availableWidthWithoutGap) - defaultWidthWithoutGap) / static_cast<float>(stretchedCount);
				localX = bounds.getX();
			}
			else {
				stretchedWidth = 0;
				localX = bounds.getXCenter() - static_cast<int32_t>(defaultWidthWithGap / 2);
			}

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKeyWidth();

				buttonWidth =
					// Not last key in row
					i < buttonIndexTo - 1
					? (
						buttonWidth == KeyboardButton::stretched
						? std::round(stretchedWidth)
						: std::round(buttonWidth * static_cast<float>(availableWidthWithoutGap))
					)
					// Last
					: (
						stretchedCount > 0 || rowIsFullOfDefaultKeys
						// Stretching to end
						? static_cast<float>(bounds.getWidth() - localX)
						// Using desired key size
						: std::round(buttonWidth * static_cast<float>(availableWidthWithoutGap))
					);

				button->arrange(Rectangle(
					bounds.getX() + localX,
					y,
					static_cast<uint16_t>(buttonWidth),
					_keyboard->getKeyHeight()
				));

				localX += static_cast<uint16_t>(buttonWidth) + _keyboard->getHorizontalKeyGap();
			}
		};

		for (size_t i = 0; i < getChildrenCount(); i++) {
			const auto button = dynamic_cast<KeyboardButton*>(getChildAt(i));

			if (button->getRow() > rowIndex) {
				arrangeRow(i);

				y += _keyboard->getKeyHeight() + _keyboard->getVerticalKeyGap();
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