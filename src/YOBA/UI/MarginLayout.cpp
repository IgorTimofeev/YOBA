#include <YOBA/UI/MarginLayout.hpp>

namespace YOBA {
	MarginLayout::MarginLayout(const Margin& margin) {
		_margin = margin;
	}

	const Margin& MarginLayout::getMargin() const {
		return _margin;
	}

	void MarginLayout::setMargin(const Margin& margin) {
		if (margin == _margin)
			return;

		_margin = margin;

		invalidate();
	}

	Size MarginLayout::onMeasure(const Size& availableSize) {
		const auto& childrenSize = Layout::onMeasure(availableSize);

		// Width
		int32_t width;

		switch (getHorizontalAlignment()) {
			case Alignment::start:
				width = childrenSize.getWidth() + _margin.getLeft();
				break;

			case Alignment::center:
				width = childrenSize.getWidth();
				break;

			case Alignment::end:
				width = childrenSize.getWidth() + _margin.getRight();
				break;

			default:
				width = childrenSize.getWidth() + _margin.getLeft() + _margin.getRight();
				break;
		}

		if (width < 0)
			width = 0;

		// Height
		int32_t height;

		switch (getVerticalAlignment()) {
			case Alignment::start:
				height = childrenSize.getHeight() + _margin.getTop();
				break;

			case Alignment::center:
				height = childrenSize.getHeight();
				break;

			case Alignment::end:
				height = childrenSize.getHeight() + _margin.getBottom();
				break;

			default:
				height = childrenSize.getHeight() + _margin.getTop() + _margin.getBottom();
				break;
		}

		if (height < 0)
			height = 0;

		return Size(
			static_cast<uint16_t>(width),
			static_cast<uint16_t>(height)
		);
	}

	void MarginLayout::onArrange(const Rectangle& bounds) {
		int32_t x = bounds.getX();
		int32_t y = bounds.getY();
		int32_t width = bounds.getWidth();
		int32_t height = bounds.getHeight();

		switch (getHorizontalAlignment()) {
			case Alignment::start:
				x = x + _margin.getLeft();
				break;

			case Alignment::center:
				break;

			case Alignment::end:
				break;

			default:
				x = x + _margin.getLeft();
				width = width - _margin.getLeft() - _margin.getRight();
				break;
		}

		if (width < 0)
			width = 0;

		switch (getVerticalAlignment()) {
			case Alignment::start:
				y = y + _margin.getTop();
				break;

			case Alignment::center:
				break;

			case Alignment::end:
				break;

			default:
				y = y + _margin.getTop();
				height = height - _margin.getTop() - _margin.getBottom();
				break;
		}

		if (height < 0)
			height = 0;

		const Rectangle boundsWithMargin {
			x,
			y,
			static_cast<uint16_t>(width),
			static_cast<uint16_t>(height)
		};

		for (const auto child : *this) {
			if (child->isVisible()) {
				child->arrange(boundsWithMargin);
			}
		}
	}
}