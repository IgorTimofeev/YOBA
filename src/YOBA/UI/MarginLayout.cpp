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
		// auto width = static_cast<int32_t>(childrenSize.getWidth());
		//
		// switch (getHorizontalAlignment()) {
		// 	case Alignment::start:
		// 		width = width + _margin.getLeft();
		// 		break;
		// 	case Alignment::center:
		// 		width = width + _margin.getLeft() + _margin.getRight();
		// 		break;
		// 	case Alignment::end:
		// 		width = width + _margin.getRight();
		// 		break;
		// 	case Alignment::stretch:
		// 		width = width + _margin.getLeft() + _margin.getRight();
		// 		break;
		// }

		auto width = static_cast<int32_t>(childrenSize.getWidth()) + _margin.getLeft() + _margin.getRight();

		if (width < 0)
			width = 0;

		// Height
		auto height = static_cast<int32_t>(childrenSize.getHeight()) + _margin.getTop() + _margin.getBottom();

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
				width = width - _margin.getLeft();
				break;

			case Alignment::center:
				break;

			case Alignment::end:
				x = x - _margin.getRight();
				width = width - _margin.getRight();
				break;

			case Alignment::stretch:
				x = x + _margin.getLeft();
				width = width - _margin.getLeft() - _margin.getRight();
				break;
		}

		switch (getHorizontalAlignment()) {
			case Alignment::start:
				y = y + _margin.getTop();
				height = height - _margin.getTop();
				break;

			case Alignment::center:
				break;

			case Alignment::end:
				y = y - _margin.getBottom();
				height = height - _margin.getBottom();
				break;

			case Alignment::stretch:
				y = y + _margin.getTop();
				height = height - _margin.getTop() - _margin.getBottom();
				break;
		}

		if (width < 0)
			width = 0;

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
