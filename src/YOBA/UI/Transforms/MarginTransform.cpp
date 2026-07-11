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

	Size MarginTransform::processAvailableSizeBeforeMeasure(const Element* element, const Size& availableSize) {
		// Shinking down available size for children if size is explicitly known (non-computed)

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

	Size MarginTransform::processMeasuredSizeAfterMeasure(const Element* element, const Size& measuredSize) {
		// Expanding up total measured size
		return Size(
			measuredSize.getWidth() + _margin.getLeft() + _margin.getRight(),
			measuredSize.getHeight() + _margin.getTop() + _margin.getBottom()
		);
	}

	Rectangle MarginTransform::processLayoutBoundsOnArrange(const Element* element, const Rectangle& bounds) {
		int32_t x = bounds.getX();
		int32_t y = bounds.getY();
		int32_t width = bounds.getWidth();
		int32_t height = bounds.getHeight();

		switch (element->getHorizontalAlignment()) {
			case Alignment::start:
				x = x + _margin.getLeft();
				break;

			case Alignment::center:
				x = x + _margin.getLeft() - _margin.getRight();
				break;

			case Alignment::end:
				x = x - _margin.getRight();
				break;

			default:
				x = x + _margin.getLeft();
				width = width - _margin.getLeft() - _margin.getRight();

				if (width < 0)
					width = 0;

				break;
		}

		switch (element->getVerticalAlignment()) {
			case Alignment::start:
				y = y + _margin.getTop();
				break;

			case Alignment::center:
				y = y + _margin.getTop() - _margin.getBottom();
				break;

			case Alignment::end:
				y = y - _margin.getBottom();
				break;

			default:
				y = y + _margin.getTop();
				height = height - _margin.getTop() - _margin.getBottom();

				if (height < 0)
					height = 0;

				break;
		}

		return {
			x,
			y,
			static_cast<uint16_t>(width),
			static_cast<uint16_t>(height)
		};
	}
}
