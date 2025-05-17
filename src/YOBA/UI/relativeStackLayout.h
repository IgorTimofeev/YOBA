#pragma once

#include <unordered_map>
#include "layout.h"
#include "stackLayout.h"

namespace YOBA {
	class RelativeStackLayout : public StackLayout {
		public:
			RelativeStackLayout() : StackLayout() {

			}

			explicit RelativeStackLayout(Orientation orientation) : StackLayout(orientation) {

			}

			explicit RelativeStackLayout(uint16_t spacing) : StackLayout(spacing) {

			}

			RelativeStackLayout(Orientation orientation, uint16_t spacing) : StackLayout(orientation, spacing) {

			}

			constexpr static float autoSize = -1;

			float getRelativeSize(const Element* child) {
				return _elementSizes.contains(child) ? _elementSizes[child] : 1;
			}

			void setRelativeSize(const Element* child, float value) {
				if (value == 1) {
					tryRemoveRelativeSize(child);
				}
				else {
					_elementSizes.insert(std::pair(child, value));
				}
			}

			bool isAutoSize(const Element* child) {
				return getRelativeSize(child) == autoSize;
			}

			void setAutoSize(const Element* child, bool value = true) {
				setRelativeSize(child, value ? autoSize : 1);
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
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
								child->measure(
									Size(
										Size::unlimited,
										availableSize.getHeight()
									)
								);

								autoSizeSum += child->getMeasuredSize().getWidth();
							}

							visibleCount++;
						}

						availableSizeWithoutSpacing =
							visibleCount == 1
							? availableSize.getWidth()
							: availableSize.getWidth() - (visibleCount - 1) * getSpacing();

						availableSizeForRelativeElements =
							autoSizeSum > availableSizeWithoutSpacing
							? availableSizeWithoutSpacing
							: availableSizeWithoutSpacing - autoSizeSum;

						// 2nd loop, measuring relative-sized children & computing total layout size
						for (const auto child : *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Relative
							if (childRelativeSize >= 0) {
								relativeIndex++;

								if (relativeIndex < relativeCount) {
									childSize = availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum);
									usedRelativeSize += childSize;
								}
								else {
									childSize = availableSizeForRelativeElements - usedRelativeSize;
								}

								child->measure(
									Size(
										childSize,
										availableSize.getHeight()
									)
								);
							}
							// Auto, already measured
							else {

							}

							childMeasuredSize = child->getMeasuredSize();

							if (childMeasuredSize.getHeight() > result.getHeight())
								result.setHeight(childMeasuredSize.getHeight());

							result.setWidth(result.getWidth() + childMeasuredSize.getWidth() + getSpacing());
						}

						if (visibleCount > 1)
							result.setWidth(result.getWidth() - getSpacing());

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
								child->measure(
									Size(
										availableSize.getWidth(),
										Size::unlimited
									)
								);

								autoSizeSum += child->getMeasuredSize().getHeight();
							}

							visibleCount++;
						}

						availableSizeWithoutSpacing =
							visibleCount == 1
							? availableSize.getHeight()
							: availableSize.getHeight() - (visibleCount - 1) * getSpacing();

						availableSizeForRelativeElements =
							autoSizeSum > availableSizeWithoutSpacing
							? availableSizeWithoutSpacing
							: availableSizeWithoutSpacing - autoSizeSum;

						// 2nd loop, measuring relative-sized children & computing total layout size
						for (const auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Relative
							if (childRelativeSize >= 0) {
								relativeIndex++;

								if (relativeIndex < relativeCount) {
									childSize = availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum);
									usedRelativeSize += childSize;
								}
								else {
									childSize = availableSizeForRelativeElements - usedRelativeSize;
								}

								child->measure(
									Size(
										availableSize.getWidth(),
										childSize
									)
								);
							}
							// Auto, already measured
							else {

							}

							childMeasuredSize = child->getMeasuredSize();

							if (childMeasuredSize.getWidth() > result.getWidth())
								result.setWidth(childMeasuredSize.getWidth());

							result.setHeight(result.getHeight() + childMeasuredSize.getHeight() + getSpacing());
						}

						if (visibleCount > 1)
							result.setHeight(result.getHeight() - getSpacing());

						break;
					}
				}

				return result;
			}

			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();

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
							visibleCount == 1
							? bounds.getWidth()
							: bounds.getWidth() - (visibleCount - 1) * getSpacing();

						availableSizeForRelativeElements =
							autoSizeSum > availableSizeWithoutSpacing
							? availableSizeWithoutSpacing
							: availableSizeWithoutSpacing - autoSizeSum;

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
									childSize = availableSizeForRelativeElements - usedRelativeSize;
								}
							}
							else {
								childSize = child->getMeasuredSize().getWidth();
							}

							child->render(renderer, Bounds(
								position,
								bounds.getY(),
								childSize,
								bounds.getHeight()
							));

							position += childSize + getSpacing();
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
							visibleCount == 1
							? bounds.getHeight()
							: bounds.getHeight() - (visibleCount - 1) * getSpacing();

						availableSizeForRelativeElements =
							autoSizeSum > availableSizeWithoutSpacing
							? availableSizeWithoutSpacing
							: availableSizeWithoutSpacing - autoSizeSum;

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
									childSize = availableSizeForRelativeElements - usedRelativeSize;
								}
							}
							else {
								childSize = child->getMeasuredSize().getHeight();
							}

							child->render(renderer, Bounds(
								bounds.getX(),
								position,
								bounds.getWidth(),
								childSize
							));

							position += childSize + getSpacing();
						}

						break;
					}
				}
			}

			void onChildRemoved(Element* child) override {
				Layout::onChildRemoved(child);

				tryRemoveRelativeSize(child);
			}

		private:
			std::unordered_map<const Element*, float> _elementSizes {};

			void tryRemoveRelativeSize(const Element* child) {
				_elementSizes.erase(child);
			}
	};
}