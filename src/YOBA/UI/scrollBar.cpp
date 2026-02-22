#include <YOBA/UI/scrollBar.h>

namespace YOBA {
	uint16_t ScrollBar::getPosition() const {
		return position;
	}

	void ScrollBar::setPosition(const uint16_t value) {
		if (value == position)
			return;

		position = value;

		invalidate();
	}

	uint16_t ScrollBar::getViewportSize() const {
		return viewportSize;
	}

	void ScrollBar::setViewportSize(const uint16_t value) {
		if (value == viewportSize)
			return;

		viewportSize = value;

		invalidate();
	}

	uint16_t ScrollBar::getContentSize() const {
		return contentSize;
	}

	void ScrollBar::setContentSize(const uint16_t value) {
		if (value == contentSize)
			return;

		contentSize = value;

		invalidate();
	}

	const Color& ScrollBar::getThumbColor() const {
		return *thumbColor;
	}

	void ScrollBar::setThumbColor(const Color& value) {
		if (&value == thumbColor)
			return;

		thumbColor = &value;

		invalidate();
	}

	void ScrollBar::onRender(Renderer& renderer, const Bounds& bounds) {
		// Thumb
		if (!thumbColor)
			return;

		const auto size = static_cast<uint16_t>(static_cast<uint64_t>(bounds.getHeight()) * static_cast<uint64_t>(viewportSize) / static_cast<uint64_t>(contentSize));
		const auto renderPos = static_cast<uint16_t>(static_cast<uint64_t>(bounds.getHeight()) * static_cast<uint64_t>(position) / static_cast<uint64_t>(contentSize));

		if (getOrientation() == Orientation::horizontal) {
			renderer.renderFilledRectangle(
				Bounds(
					bounds.getX() + renderPos,
					bounds.getY(),
					size,
					bounds.getHeight()
				),
				getCornerRadius(),
				*thumbColor
			);
		}
		else {
			renderer.renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY() + renderPos,
					bounds.getWidth(),
					size
				),
				getCornerRadius(),
				*thumbColor
			);
		}
	}
}
