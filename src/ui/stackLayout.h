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

				bool haveVisibleChildren = false;

				switch (getOrientation()) {
					case Orientation::Horizontal:
						for (auto child : _children) {
							if (!child->isVisible())
								continue;

							child->measure(
								screenBuffer,
								Size(
									Size::Calculated,
									availableSize.getHeight()
								)
							);

							result.setWidth(result.getWidth() + child->getMeasuredSize().getWidth() + getSpacing());

							if (child->getMeasuredSize().getHeight() > result.getHeight())
								result.setHeight(child->getMeasuredSize().getHeight());

							haveVisibleChildren = true;
						}

						if (haveVisibleChildren)
							result.setWidth(result.getWidth() - getSpacing());

						break;

					case Orientation::Vertical:
						for (auto child : _children) {
							if (!child->isVisible())
								continue;

							child->measure(
								screenBuffer,
								Size(
									availableSize.getWidth(),
									Size::Calculated
								)
							);

							if (child->getMeasuredSize().getWidth() > result.getWidth())
								result.setWidth(child->getMeasuredSize().getWidth());

							result.setHeight(result.getHeight() + child->getMeasuredSize().getHeight() + getSpacing());

							haveVisibleChildren = true;
						}

						if (haveVisibleChildren > 0)
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
								child->getMeasuredSize().getWidth(),
								bounds.getHeight()
							));

							position += child->getMeasuredSize().getWidth() + getSpacing();
						}

						break;

					case Orientation::Vertical:
						position = bounds.getY();

						for (auto child : _children) {
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
			uint16_t _spacing = 0;
			Orientation _orientation = Orientation::Vertical;
	};
}