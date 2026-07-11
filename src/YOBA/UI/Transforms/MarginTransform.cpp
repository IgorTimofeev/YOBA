#include <YOBA/UI/Transforms/MarginTransform.hpp>

namespace YOBA {
	MarginTransform::MarginTransform(const Margin& value): _margin(value) {

	}

	MarginTransform::MarginTransform(): _margin(0) {

	}

	const Margin& MarginTransform::getMargin() const {
		return _margin;
	}

	void MarginTransform::setMargin(const Margin& value) {
		_margin = value;
	}

	Size MarginTransform::processAvailableSizeForMeasure(Element* element, const Size& availableSize) {
		// Reducing available size for children if size itself is explicitly known (non-computed)

		// Horizontal
		int32_t width = availableSize.getWidth();

		if (width != Size::computed) {
			width = width - _margin.getLeft() - _margin.getRight();

			if (width < 0)
				width = 0;
		}

		// Vertical
		int32_t height = availableSize.getHeight();

		if (height != Size::computed) {
			height = height - _margin.getTop() - _margin.getBottom();

			if (height < 0)
				height = 0;
		}

		return {
			static_cast<uint16_t>(width),
			static_cast<uint16_t>(height)
		};
	}

	Size MarginTransform::processMeasuredSize(Element* element, const Size& measuredSize) {
		// Adding margin to measured size
		return Size(
			static_cast<int32_t>(measuredSize.getWidth()) + _margin.getLeft() + _margin.getRight(),
			static_cast<int32_t>(measuredSize.getHeight()) + _margin.getTop() + _margin.getBottom()
		);
	}

	Rectangle MarginTransform::processLayoutBounds(Element* element, const Rectangle& layoutBounds) {
		// Translating by margin & removing it from size
		return {
			layoutBounds.getX() + _margin.getLeft(),
			layoutBounds.getY() + _margin.getTop(),
			static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(layoutBounds.getWidth()) - _margin.getLeft() - _margin.getRight(), 0)),
			static_cast<uint16_t>(std::max<int32_t>(static_cast<int32_t>(layoutBounds.getHeight()) - _margin.getTop() - _margin.getBottom(), 0))
		};
	}
}
