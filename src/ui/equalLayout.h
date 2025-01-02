#pragma once

#include "stackContainer.h"
#include "set"
#include "../size.h"

namespace yoba {
	class EqualLayout : public StackContainer {
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
			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				auto result = Size();

				uint16_t spacingSubstraction =
					getChildrenCount() > 1
					? (getChildrenCount() - 1) * getSpacing()
					: 0;

				uint16_t availableWithoutSpacing;
				uint16_t equalSize;

				switch (getOrientation()) {
					case Orientation::Horizontal:
						for (auto child : *this) {
							child->measure(
								screenBuffer,
								Size(
									Size::Infinity,
									availableSize.getHeight()
								)
							);

							result.setWidth(result.getWidth() + child->getMeasuredSize().getWidth() + getSpacing());

							if (child->getMeasuredSize().getHeight() > result.getHeight())
								result.setHeight(child->getMeasuredSize().getHeight());
						}

						if (getChildrenCount() > 0)
							result.setWidth(result.getWidth() - getSpacing());

						break;

					case Orientation::Vertical:
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
							child->measure(
								screenBuffer,
								Size(
									availableSize.getWidth(),
									getChildAutoSize(child) ? Size::Infinity : equalSize
								)
							);

							if (child->getMeasuredSize().getWidth() > result.getWidth())
								result.setWidth(child->getMeasuredSize().getWidth());

							result.setHeight(result.getHeight() + child->getMeasuredSize().getHeight() + getSpacing());
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
					case Orientation::Horizontal:
						position = bounds.getX();

						for (auto child : *this) {
							child->arrange(Bounds(
								position,
								bounds.getY(),
								child->getMeasuredSize().getWidth(),
								bounds.getHeight()
							));

							position += child->getMeasuredSize().getWidth() + getSpacing();
						}

						break;

					case Orientation::Vertical:
						position = bounds.getY();

						for (auto child : *this) {
							child->arrange(Bounds(
								bounds.getX(),
								position,
								bounds.getWidth(),
								child->getMeasuredSize().getHeight()
							));

							position += child->getMeasuredSize().getHeight() + getSpacing();
						}

						break;
				}
			}

		private:
			std::set<Element*> _childrenAutoSizes {};
			int32_t _spacing = 10;
			Orientation _orientation = Orientation::Vertical;
	};
}