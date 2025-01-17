#pragma once

#include <unordered_set>
#include "layout.h"
#include "stackLayout.h"

namespace yoba::ui {
	class EqualStackLayout : public StackLayout {
		public:
			EqualStackLayout() : StackLayout() {

			}

			EqualStackLayout(Orientation orientation) : StackLayout(orientation) {

			}

			EqualStackLayout(uint16_t spacing) : StackLayout(spacing) {

			}

			EqualStackLayout(Orientation orientation, uint16_t spacing) : StackLayout(orientation, spacing) {

			}

			void setFit(Element* element, bool value = true) {
				if (value) {
					_fitElements.insert(element);
				}
				else {
					tryRemoveComputedSizeElement(element);
				}
			}
			
			bool isFit(Element* element) {
				return _fitElements.find(element) != _fitElements.end();
			}

		protected:
			Size onMeasure(Renderer* renderer, const Size& availableSize) override {
				auto result = Size();

				size_t
					visibleCount = 0,
					fitCount = 0,
					equalCount;

				uint16_t
					fitSize = 0,
					equalSize,
					sizeWithoutSpacing;

				Size measuredSize;

				switch (getOrientation()) {
					case Orientation::horizontal:
						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							if (isFit(child)) {
								child->measure(
									renderer,
									Size(
										Size::infinity,
										availableSize.getHeight()
									)
								);

								fitSize += child->getMeasuredSize().getWidth();

								fitCount++;
							}

							visibleCount++;
						}

						equalCount = visibleCount - fitCount;

						sizeWithoutSpacing =
							visibleCount == 1
							? availableSize.getWidth()
							: availableSize.getWidth() - (visibleCount - 1) * getSpacing();

						equalSize =
							equalCount == 0
							? 0
							: (sizeWithoutSpacing - fitSize) / equalCount;

						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							if (!isFit(child)) {
								child->measure(
									renderer,
									Size(
										equalSize,
										availableSize.getHeight()
									)
								);
							}

							measuredSize = child->getMeasuredSize();

							if (measuredSize.getHeight() > result.getHeight())
								result.setHeight(measuredSize.getHeight());

							result.setWidth(result.getWidth() + measuredSize.getWidth() + getSpacing());
						}

						if (visibleCount > 1)
							result.setWidth(result.getWidth() - getSpacing());

						break;

					case Orientation::vertical:
						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							if (isFit(child)) {
								child->measure(
									renderer,
									Size(
										availableSize.getWidth(),
										Size::infinity
									)
								);

								fitSize += child->getMeasuredSize().getHeight();

								fitCount++;
							}

							visibleCount++;
						}

						equalCount = visibleCount - fitCount;

						sizeWithoutSpacing =
							visibleCount == 1
							? availableSize.getHeight()
							: availableSize.getHeight() - (visibleCount - 1) * getSpacing();

						equalSize =
							equalCount == 0
							? 0
							: (sizeWithoutSpacing - fitSize) / equalCount;

						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							if (!isFit(child)) {
								child->measure(
									renderer,
									Size(
										availableSize.getWidth(),
										equalSize
									)
								);
							}

							measuredSize = child->getMeasuredSize();

							if (measuredSize.getWidth() > result.getWidth())
								result.setWidth(measuredSize.getWidth());

							result.setHeight(result.getHeight() + measuredSize.getHeight() + getSpacing());
						}

						if (visibleCount > 1)
							result.setHeight(result.getHeight() - getSpacing());

						break;
				}

				return result;
			}

			void onArrange(const Bounds& bounds) override {
				size_t
					visibleCount = 0,
					fitCount = 0,
					equalCount;

				uint16_t
					fitSize = 0,
					equalSize,
					childSize,
					sizeWithoutSpacing;
				
				int32_t position;

				switch (getOrientation()) {
					case Orientation::horizontal:
						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							if (isFit(child)) {
								fitSize += child->getMeasuredSize().getWidth();

								fitCount++;
							}

							visibleCount++;
						}

						equalCount = visibleCount - fitCount;

						sizeWithoutSpacing =
							visibleCount == 1
							? bounds.getWidth()
							: bounds.getWidth() - (visibleCount - 1) * getSpacing();

						equalSize =
							equalCount == 0
							? 0
							: (sizeWithoutSpacing - fitSize) / equalCount;

						position = bounds.getX();

						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							childSize =
								isFit(child)
								? child->getMeasuredSize().getWidth()
								: equalSize;

							child->arrange(Bounds(
								position,
								bounds.getY(),
								childSize,
								bounds.getHeight()
							));

							position += childSize + getSpacing();
						}

						break;

					case Orientation::vertical:
						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							if (isFit(child)) {
								fitSize += child->getMeasuredSize().getHeight();

								fitCount++;
							}

							visibleCount++;
						}

						equalCount = visibleCount - fitCount;

						sizeWithoutSpacing =
							visibleCount == 1
							? bounds.getHeight()
							: bounds.getHeight() - (visibleCount - 1) * getSpacing();

						equalSize =
							equalCount == 0
							? 0
							: (sizeWithoutSpacing - fitSize) / equalCount;

						position = bounds.getY();

						for (auto child : *this) {
							if (!child->isVisible())
								continue;

							childSize =
								isFit(child)
								? child->getMeasuredSize().getHeight()
								: equalSize;

							child->arrange(Bounds(
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

				tryRemoveComputedSizeElement(element);
			}

		private:
			std::unordered_set<Element*> _fitElements {};

			void tryRemoveComputedSizeElement(Element* element) {
				auto it = _fitElements.find(element);

				if (it != _fitElements.end())
					_fitElements.erase(it);
			}
	};
}