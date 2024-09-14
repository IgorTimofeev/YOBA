#pragma once

#include "stack_layout.h"
#include "set"

namespace yoba {
	class EqualLayout : public StackLayout {
		public:
			void setChildAutoSize(Element* element, bool value) {
				if (value) {
					_childrenAutoSizes.insert(element);
				}
				else {
					_childrenAutoSizes.erase(element);
				}
			}

			bool getChildAutoSize(Element* element) {
				return _childrenAutoSizes.find(element) != _childrenAutoSizes.end();
			}

		protected:
			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				auto result = Size();

				Size childSize;

				uint16_t spacingSubstraction =
					getChildrenCount() > 1
					? (getChildrenCount() - 1) * getSpacing()
					: 0;

				uint16_t availableWithoutSpacing;
				uint16_t equalSize;

				switch (getOrientation()) {
					case horizontal:
						for (auto child : *this) {
							childSize = child->measure(
								display,
								Size(
									Size::infinity,
									availableSize.getHeight()
								)
							);

							result.setWidth(result.getWidth() + childSize.getWidth() + getSpacing());

							if (childSize.getHeight() > result.getHeight())
								result.setHeight(childSize.getHeight());
						}

						if (getChildrenCount() > 0)
							result.setWidth(result.getWidth() - getSpacing());

						break;

					case vertical:
						availableWithoutSpacing = availableSize.getHeight() - spacingSubstraction;

						equalSize =
							getChildrenCount() == 0
							? 0
							: availableWithoutSpacing / getChildrenCount();

						for (auto child : *this) {
							if (getChildAutoSize(child)) {

							}
							else {

							}
						}

						for (auto child : *this) {
							childSize = child->measure(
								display,
								Size(
									availableSize.getWidth(),
									getChildAutoSize(child) ? Size::infinity : equalSize
								)
							);

							if (childSize.getWidth() > result.getWidth())
								result.setWidth(childSize.getWidth());

							result.setHeight(result.getHeight() + childSize.getHeight() + getSpacing());
						}

						if (getChildrenCount() > 0)
							result.setHeight(result.getHeight() - getSpacing());

						break;
				}

				return result;
			}

			void onArrange(const Bounds& bounds) override {
				int32_t position;

				switch (getOrientation()) {
					case horizontal:
						position = bounds.getX();

						for (auto child : *this) {
							child->arrange(Bounds(
								position,
								bounds.getY(),
								child->getDesiredSize().getWidth(),
								bounds.getHeight()
							));

							position += child->getDesiredSize().getWidth() + getSpacing();
						}

						break;

					case vertical:
						position = bounds.getY();

						for (auto child : *this) {
							child->arrange(Bounds(
								bounds.getX(),
								position,
								bounds.getWidth(),
								child->getDesiredSize().getHeight()
							));

							position += child->getDesiredSize().getHeight() + getSpacing();
						}

						break;
				}
			}

		private:
			std::set<Element*> _childrenAutoSizes {};
			int32_t _spacing = 10;
			Orientation _orientation = Orientation::vertical;
	};
}