#pragma once

#include <unordered_set>
#include "layout.h"
#include "stackLayout.h"

namespace yoba::ui {
	class EqualLayout : public StackLayout {
		public:
			EqualLayout() : StackLayout() {

			}

			explicit EqualLayout(Orientation orientation) : StackLayout(orientation) {

			}

			explicit EqualLayout(uint16_t spacing) : StackLayout(spacing) {

			}

			EqualLayout(Orientation orientation, uint16_t spacing) : StackLayout(orientation, spacing) {

			}

			void setFit(Element* element, bool value = true) {
				if (value) {
					_fitElements.insert(element);
				}
				else {
					tryRemoveFitElement(element);
				}
			}
			
			bool isFit(Element* element) {
				return _fitElements.find(element) != _fitElements.end();
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
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

			void onRender(Renderer* renderer, const Bounds& bounds) override {
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
			std::unordered_set<Element*> _fitElements;

			void tryRemoveFitElement(Element* element) {
				auto it = _fitElements.find(element);

				if (it != _fitElements.end())
					_fitElements.erase(it);
			}
	};
}