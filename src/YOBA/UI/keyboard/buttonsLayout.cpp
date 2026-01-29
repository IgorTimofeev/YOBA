#include <YOBA/main/math.h>

#include <YOBA/UI/keyboard/buttonsLayout.h>
#include <YOBA/UI/keyboard/keyboard.h>
#include <YOBA/UI/application.h>

namespace YOBA {
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

	void KeyboardButtonsLayout::onRender(Renderer* renderer, const Bounds& bounds) {
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
			KeyboardButton* button;
			float buttonWidth;

			for (size_t i = buttonIndexFrom; i < buttonIndexTo; i++) {
				button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKeyWidth();

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
				button = dynamic_cast<KeyboardButton*>(getChildAt(i));

				buttonWidth = button->getKeyWidth();

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
}