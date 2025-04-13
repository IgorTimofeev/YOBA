#pragma once

#include <unordered_set>
#include <optional>
#include "layout.h"
#include "stackLayout.h"

namespace yoba::ui {
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

			constexpr static const float autoSize = -1;

			float getRelativeSize(Element* child) {
				return _elementSizes.contains(child) ? _elementSizes[child] : 1;
			}

			void setRelativeSize(Element* child, float value) {
				if (value == 1) {
					tryRemoveFitElement(child);
				}
				else {
					_elementSizes.insert(std::pair(child, value));
				}
			}

			bool isAutoSize(Element* child) {
				return getRelativeSize(child) == autoSize;
			}

			void setAutoSize(Element* child, bool value = true) {
				setRelativeSize(child, value ? autoSize : 1);
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				auto result = Size();

				size_t
					visibleCount = 0;

				uint16_t
					autoSizeSum = 0,
					availableSizeWithoutSpacing,
					availableSizeForRelativeElements;

				float
					childRelativeSize,
					relativeSizesSum = 0;

				Size childMeasuredSize;

				switch (getOrientation()) {
					case Orientation::horizontal: {
						// 1st loop, computing sum of relative sizes & sum of auto sizes
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Non-auto
							if (childRelativeSize >= 0) {
								relativeSizesSum += childRelativeSize;
							}
							// Auto
							else {
								child->measure(
									Size(
										Size::infinity,
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
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Non-auto
							if (childRelativeSize >= 0) {
								child->measure(
									Size(
										availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum),
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
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							if (childRelativeSize >= 0) {
								relativeSizesSum += childRelativeSize;
							}
							else {
								child->measure(
									Size(
										availableSize.getWidth(),
										Size::infinity
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
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Non-auto
							if (childRelativeSize >= 0) {
								child->measure(
									Size(
										availableSize.getWidth(),
										availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum)
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

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				size_t
					visibleCount = 0;

				uint16_t
					autoSizeSum = 0,
					availableSizeWithoutSpacing,
					availableSizeForRelativeElements,
					childSize;

				float
					childRelativeSize,
					relativeSizesSum = 0;

				Size measuredSize;
				
				int32_t position;

				switch (getOrientation()) {
					case Orientation::horizontal:
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Non-auto
							if (childRelativeSize >= 0) {
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

						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							childSize =
								childRelativeSize >= 0
								? availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum)
								: child->getMeasuredSize().getWidth();

							child->render(renderer, Bounds(
								position,
								bounds.getY(),
								childSize,
								bounds.getHeight()
							));

							position += childSize + getSpacing();
						}

						break;

					case Orientation::vertical:
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							// Non-auto
							if (childRelativeSize >= 0) {
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

						position = bounds.getX();

						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							childRelativeSize = getRelativeSize(child);

							childSize =
								childRelativeSize >= 0
								? availableSizeForRelativeElements * (childRelativeSize / relativeSizesSum)
								: child->getMeasuredSize().getHeight();

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

			void onChildRemoved(Element* element) override {
				Layout::onChildRemoved(element);

				tryRemoveFitElement(element);
			}

		private:
			std::unordered_map<Element*, float> _elementSizes;

			void tryRemoveFitElement(Element* element) {
				if (_elementSizes.contains(element))
					_elementSizes.erase(element);
			}
	};
}