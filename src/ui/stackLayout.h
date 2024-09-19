#pragma once

#include "layout.h"

namespace yoba {
	class StackLayout : public Layout {
		public:
			Orientation getOrientation() const {
				return _orientation;
			}

			void setOrientation(Orientation orientation) {
				_orientation = orientation;
			}

			uint16_t getSpacing() const {
				return _spacing;
			}

			void setSpacing(uint16_t value) {
				_spacing = value;
			}

		protected:
			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override {
				auto result = Size();

				size_t visibleChildrenCount = 0;
				Size childSize;

				switch (getOrientation()) {
					case Orientation::Horizontal:
						for (auto child : _children) {
							if (!child->isVisible())
								continue;

							childSize = child->measure(
								screenBuffer,
								Size(
									Size::Calculated,
									availableSize.getHeight()
								)
							);

							result.setWidth(result.getWidth() + childSize.getWidth() + getSpacing());

							if (childSize.getHeight() > result.getHeight())
								result.setHeight(childSize.getHeight());

							visibleChildrenCount++;
						}

						if (visibleChildrenCount > 0)
							result.setWidth(result.getWidth() - getSpacing());

						break;

					case Orientation::Vertical:
						for (auto child : _children) {
							if (!child->isVisible())
								continue;

							childSize = child->measure(
								screenBuffer,
								Size(
									availableSize.getWidth(),
									Size::Calculated
								)
							);

							if (childSize.getWidth() > result.getWidth())
								result.setWidth(childSize.getWidth());

							result.setHeight(result.getHeight() + childSize.getHeight() + getSpacing());

							visibleChildrenCount++;
						}

						if (visibleChildrenCount > 0)
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

						for (auto child : _children) {
							child->arrange(Bounds(
								position,
								bounds.getY(),
								child->getDesiredSize().getWidth(),
								bounds.getHeight()
							));

							position += child->getDesiredSize().getWidth() + getSpacing();
						}

						break;

					case Orientation::Vertical:
						position = bounds.getY();

						for (auto child : _children) {
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
			uint16_t _spacing = 0;
			Orientation _orientation = Orientation::Vertical;
	};
}