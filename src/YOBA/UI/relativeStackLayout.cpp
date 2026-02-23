#include <YOBA/UI/relativeStackLayout.h>

namespace YOBA {
	RelativeStackLayout::RelativeStackLayout(): StackLayout() {

	}

	RelativeStackLayout::RelativeStackLayout(const Orientation orientation): StackLayout(orientation) {

	}

	RelativeStackLayout::RelativeStackLayout(const uint16_t spacing): StackLayout(spacing) {

	}

	RelativeStackLayout::RelativeStackLayout(const Orientation orientation, const uint16_t spacing): StackLayout(orientation, spacing) {

	}

	float RelativeStackLayout::getRelativeSize(const Element* child) {
		return _elementSizes.contains(child) ? _elementSizes[child] : 1;
	}

	void RelativeStackLayout::setRelativeSize(const Element* child, float value) {
		if (value == 1) {
			tryRemoveRelativeSize(child);
		}
		else {
			_elementSizes.insert_or_assign(child, value);
		}
	}

	bool RelativeStackLayout::isAutoSize(const Element* child) {
		return getRelativeSize(child) == autoSize;
	}

	void RelativeStackLayout::setAutoSize(const Element* child, const bool value) {
		setRelativeSize(child, value ? autoSize : 1);
	}

	Size RelativeStackLayout::onMeasure(const Size& availableSize) {
		auto result = Size();

		size_t
		visibleCount = 0,
		relativeIndex = 0,
		relativeCount = 0;

		uint16_t
		autoSizeSum = 0,
		availableSizeWithoutSpacing,
		availableSizeForRelativeElements,
		childSize;

		uint32_t usedRelativeSize = 0;

		float
		childRelativeSize,
		relativeSizesSum = 0;

		Size childMeasuredSize;

		switch (getOrientation()) {
			case Orientation::horizontal: {
				// 1st loop, computing sum of relative sizes & sum of auto sizes
				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					// Non-auto
					if (childRelativeSize >= 0) {
						relativeSizesSum += childRelativeSize;
						relativeCount++;
					}
					// Auto
					else {
						child->measure(availableSize);

						autoSizeSum += child->getMeasuredSize().getWidth();
					}

					visibleCount++;
				}

				availableSizeWithoutSpacing =
				visibleCount < 2
					? availableSize.getWidth()
					: availableSize.getWidth() - (visibleCount - 1) * getGap();

				availableSizeForRelativeElements =
					availableSizeWithoutSpacing >= autoSizeSum
					? availableSizeWithoutSpacing - autoSizeSum
					: 0;

				// 2nd loop, measuring relative-sized children & computing total layout size
				for (const auto child : *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					// Relative
					if (childRelativeSize >= 0) {
						relativeIndex++;

						if (relativeIndex < relativeCount) {
							childSize = static_cast<uint16_t>(static_cast<float>(availableSizeForRelativeElements) * (childRelativeSize / relativeSizesSum));
							usedRelativeSize += childSize;
						}
						else {
							childSize =
								availableSizeForRelativeElements >= usedRelativeSize
								? availableSizeForRelativeElements - usedRelativeSize
								: 0;
						}

						child->measure(Size(childSize, availableSize.getHeight()));
					}
					// Auto, already measured
					else {

					}

					childMeasuredSize = child->getMeasuredSize();

					if (childMeasuredSize.getHeight() > result.getHeight())
						result.setHeight(childMeasuredSize.getHeight());

					result.setWidth(result.getWidth() + childMeasuredSize.getWidth() + getGap());
				}

				if (visibleCount > 1)
					result.setWidth(result.getWidth() - getGap());

				break;
			}
			case Orientation::vertical: {
				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					if (childRelativeSize >= 0) {
						relativeSizesSum += childRelativeSize;
						relativeCount++;
					}
					else {
						child->measure(availableSize);

						autoSizeSum += child->getMeasuredSize().getHeight();
					}

					visibleCount++;
				}

				availableSizeWithoutSpacing =
					visibleCount < 2
					? availableSize.getHeight()
					: availableSize.getHeight() - (visibleCount - 1) * getGap();

				availableSizeForRelativeElements =
					availableSizeWithoutSpacing >= autoSizeSum
					? availableSizeWithoutSpacing - autoSizeSum
					: 0;

				// 2nd loop, measuring relative-sized children & computing total layout size
				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					// Relative
					if (childRelativeSize >= 0) {
						relativeIndex++;

						if (relativeIndex < relativeCount) {
							childSize = static_cast<uint16_t>(static_cast<float>(availableSizeForRelativeElements) * (childRelativeSize / relativeSizesSum));
							usedRelativeSize += childSize;
						}
						else {
							childSize =
								availableSizeForRelativeElements >= usedRelativeSize
								? availableSizeForRelativeElements - usedRelativeSize
								: 0;
						}

						child->measure(Size(availableSize.getWidth(), childSize));
					}
					// Auto, already measured
					else {

					}

					childMeasuredSize = child->getMeasuredSize();

					if (childMeasuredSize.getWidth() > result.getWidth())
						result.setWidth(childMeasuredSize.getWidth());

					result.setHeight(result.getHeight() + childMeasuredSize.getHeight() + getGap());
				}

				if (visibleCount > 1)
					result.setHeight(result.getHeight() - getGap());

				break;
			}
		}

		return result;
	}

	void RelativeStackLayout::onRender(Renderer& renderer, const Bounds& bounds) {
		size_t
		visibleCount = 0,
		relativeCount = 0,
		relativeIndex = 0;

		uint16_t
		autoSizeSum = 0,
		availableSizeWithoutSpacing,
		availableSizeForRelativeElements,
		childSize;

		uint32_t
		usedRelativeSize = 0;

		float
		childRelativeSize,
		relativeSizesSum = 0;

		int32_t position;

		switch (getOrientation()) {
			case Orientation::horizontal: {
				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					// Relative
					if (childRelativeSize >= 0) {
						relativeCount++;
						relativeSizesSum += childRelativeSize;
					}
					// Auto
					else {
						autoSizeSum += child->getMeasuredSize().getWidth();
					}

					visibleCount++;
				}

				availableSizeWithoutSpacing =
				visibleCount < 2
					? bounds.getWidth()
					: bounds.getWidth() - (visibleCount - 1) * getGap();

				availableSizeForRelativeElements =
				availableSizeWithoutSpacing >= autoSizeSum
					? availableSizeWithoutSpacing - autoSizeSum
					: 0;

				position = bounds.getX();

				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					if (childRelativeSize >= 0) {
						if (relativeIndex < relativeCount) {
							childSize = availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum);
							usedRelativeSize += childSize;
							relativeIndex++;
						}
						else {
							childSize =
							availableSizeForRelativeElements >= usedRelativeSize
								? availableSizeForRelativeElements - usedRelativeSize
								: 0;
						}
					}
					else {
						childSize = child->getMeasuredSize().getWidth();
					}

					if (childSize > bounds.getWidth())
						childSize = bounds.getWidth();

					child->render(renderer, Bounds(
						              position,
						              bounds.getY(),
						              childSize,
						              bounds.getHeight()
					              ));

					position += childSize + getGap();
				}

				break;
			}
			case Orientation::vertical: {
				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					// Relative
					if (childRelativeSize >= 0) {
						relativeCount++;
						relativeSizesSum += childRelativeSize;
					}
					// Auto
					else {
						autoSizeSum += child->getMeasuredSize().getHeight();
					}

					visibleCount++;
				}

				availableSizeWithoutSpacing =
				visibleCount < 2
					? bounds.getHeight()
					: bounds.getHeight() - (visibleCount - 1) * getGap();

				availableSizeForRelativeElements =
				availableSizeWithoutSpacing >= autoSizeSum
					? availableSizeWithoutSpacing - autoSizeSum
					: 0;

				position = bounds.getY();

				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					childRelativeSize = getRelativeSize(child);

					if (childRelativeSize >= 0) {
						relativeIndex++;

						if (relativeIndex < relativeCount) {
							childSize = availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum);
							usedRelativeSize += childSize;
						}
						else {
							childSize =
							availableSizeForRelativeElements >= usedRelativeSize
								? availableSizeForRelativeElements - usedRelativeSize
								: 0;
						}
					}
					else {
						childSize = child->getMeasuredSize().getHeight();
					}

					if (childSize > bounds.getHeight())
						childSize = bounds.getHeight();

					child->render(renderer, Bounds(
						              bounds.getX(),
						              position,
						              bounds.getWidth(),
						              childSize
					              ));

					position += childSize + getGap();
				}

				break;
			}
		}
	}

	void RelativeStackLayout::tryRemoveRelativeSize(const Element* child) {
		_elementSizes.erase(child);
	}
}
