#include <YOBA/UI/WrapLayout.hpp>

namespace YOBA {
	Size WrapLayout::onMeasure(const Size& availableSize) {
		uint16_t
		width = 0,
		height = 0,
		lineWidthWithSpacing = 0,
		lineHeight = 0;

		for (const auto child : *this) {
			child->measure({
				availableSize.getWidth(),
				Size::computed
			});

			const auto& childSize = child->getMeasuredSize();

			// Overflow
			if (lineWidthWithSpacing + childSize.getWidth() > availableSize.getWidth()) {
				if (lineWidthWithSpacing > 0) {
					width = std::max(width, static_cast<uint16_t>(lineWidthWithSpacing - getHorizontalGap()));
					height += height > 0 ? getVerticalGap() + lineHeight : lineHeight;
				}

				lineWidthWithSpacing = childSize.getWidth();
				lineHeight = childSize.getHeight();
			}
			// Normal
			else {
				lineWidthWithSpacing += childSize.getWidth() + getHorizontalGap();

				if (childSize.getHeight() > lineHeight)
					lineHeight = childSize.getHeight();
			}
		}

		// Handling unprocessed line
		if (lineWidthWithSpacing > 0) {
			width = std::max(width, static_cast<uint16_t>(lineWidthWithSpacing - getHorizontalGap()));
			height += height > 0 ? getVerticalGap() + lineHeight : lineHeight;
		}

		return { width, height };
	}

	void WrapLayout::onArrange(const Rectangle& bounds) {
		int32_t x = 0;
		int32_t y = 0;
		uint16_t lineHeight = 0;

		for (const auto child : *this) {
			if (!child->isVisible())
				continue;

			const auto& childSize = child->getMeasuredSize();

			// Overflow
			if (x + childSize.getWidth() > bounds.getWidth()) {
				if (lineHeight > 0)
					y += getVerticalGap() + lineHeight;

				child->arrange(Rectangle(
					bounds.getX(),
					bounds.getY() + y,
					childSize.getWidth(),
					childSize.getHeight()
				));

				x = childSize.getWidth() + getHorizontalGap();
				lineHeight = childSize.getHeight();
			}
			// Normal
			else {
				child->arrange(Rectangle(
					bounds.getX() + x,
					bounds.getY() + y,
					childSize.getWidth(),
					childSize.getHeight()
				));

				x += childSize.getWidth() + getHorizontalGap();

				if (childSize.getHeight() > lineHeight)
					lineHeight = childSize.getHeight();
			}
		}
	}
}
