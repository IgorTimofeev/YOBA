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

	Size MarginTransform::computeAvailableSizeBeforeMeasure(Element* element, const Size& availableSize) {
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

	Size MarginTransform::computeMeasuredSizeAfterMeasure(Element* element, const Size& measuredSize) {
		const auto computeMeasureShit = [](
			const uint16_t size,
			const uint16_t desiredSize,
			const int32_t marginStartClamped,
			const int32_t marginEndClamped
		) {
			int32_t newSize = 0;

			if (size == Size::computed) {
				newSize = static_cast<int32_t>(desiredSize);
			}
			else {
				newSize = size;
			}

			newSize = newSize + marginStartClamped + marginEndClamped;

			return newSize;
		};

		const auto& size = element->getSize();

		return Size(
			computeMeasureShit(
				size.getWidth(),
				measuredSize.getWidth(),
				_margin.getLeft(),
				_margin.getRight()
			),
			computeMeasureShit(
				size.getHeight(),
				measuredSize.getHeight(),
				_margin.getTop(),
				_margin.getBottom()
			)
		);
	}

		Rectangle MarginTransform::computeLayoutBoundsOnArrange(Element* element, const Rectangle& parentBounds) {
		int32_t x = parentBounds.getX();
		int32_t y = parentBounds.getY();
		int32_t width = parentBounds.getWidth();
		int32_t height = parentBounds.getHeight();

		const auto computeArrangeShit = [](
			const Alignment alignment,
			const int32_t boundsStart,
			const uint16_t boundsSize,

			const uint16_t size,
			const uint16_t measuredSize,

			const int32_t marginStart,
			const int32_t marginEnd,

			int32_t& newPosition,
			int32_t& newSize
		) {
			switch (alignment) {
				case Alignment::start:
					newSize = static_cast<int32_t>(measuredSize);

					if (marginStart > 0)
						newSize -= marginStart;

					if (marginEnd > 0)
						newSize -= marginEnd;

					if (newSize < 0)
						newSize = 0;

					newPosition = boundsStart + marginStart;

					break;

				case Alignment::center:
					newSize = static_cast<int32_t>(measuredSize);

					if (marginStart > 0)
						newSize -= marginStart;

					if (marginEnd > 0)
						newSize -= marginEnd;

					if (newSize < 0)
						newSize = 0;

					newPosition = boundsStart + marginStart - marginEnd + boundsSize / 2 - newSize / 2;

					break;

				case Alignment::end:
					newSize = static_cast<int32_t>(measuredSize);

					if (marginStart > 0)
						newSize -= marginStart;

					if (marginEnd > 0)
						newSize -= marginEnd;

					if (newSize < 0)
						newSize = 0;

					newPosition = boundsStart + boundsSize - marginEnd - newSize;

					break;

				case Alignment::stretch:
					if (size == Size::computed) {
						newSize = boundsSize;
					}
					else {
						newSize = static_cast<int32_t>(measuredSize);
					}

					newSize = newSize - marginStart;

					if (marginEnd > 0)
						newSize -= marginEnd;

					if (newSize < 0)
						newSize = 0;

					newPosition = boundsStart + marginStart;

					break;
			}
		};

		const auto& size = element->getSize();
		const auto& measuredSize = element->getMeasuredSize();

		computeArrangeShit(
			element->getHorizontalAlignment(),
			parentBounds.getX(),
			parentBounds.getWidth(),

			size.getWidth(),
			measuredSize.getWidth(),

			_margin.getLeft(),
			_margin.getRight(),

			x,
			width
		);

		computeArrangeShit(
			element->getVerticalAlignment(),
			parentBounds.getY(),
			parentBounds.getHeight(),

			size.getHeight(),
			measuredSize.getHeight(),

			_margin.getTop(),
			_margin.getBottom(),

			y,
			height
		);

		return {
			x,
			y,
			static_cast<uint16_t>(width),
			static_cast<uint16_t>(height)
		};
	}
}
