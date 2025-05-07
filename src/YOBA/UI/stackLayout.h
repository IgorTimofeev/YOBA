#pragma once

#include "layout.h"
#include "YOBA/UI/traits/orientationElement.h"
#include "YOBA/UI/traits/spacingElement.h"

namespace YOBA {
	class StackLayout : public Layout, public OrientationElement, public SpacingElement {
		public:
			StackLayout() = default;

			explicit StackLayout(Orientation orientation) {
				setOrientation(orientation);
			}

			explicit StackLayout(uint16_t spacing) {
				setSpacing(spacing);
			}

			StackLayout(Orientation orientation, uint16_t spacing) {
				setOrientation(orientation);
				setSpacing(spacing);
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				auto result = Size();

				size_t visibleChildrenCount = 0;

				switch (getOrientation()) {
					case Orientation::horizontal: {
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							child->measure(
								Size(
									Size::unlimited,
									availableSize.getHeight()
								)
							);

							const auto& childSize = child->getMeasuredSize();

							result.setWidth(result.getWidth() + childSize.getWidth() + getSpacing());

							if (childSize.getHeight() > result.getHeight())
								result.setHeight(childSize.getHeight());

							visibleChildrenCount++;
						}

						if (visibleChildrenCount > 0)
							result.setWidth(result.getWidth() - getSpacing());

						break;
					}
					case Orientation::vertical: {
						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							child->measure(
								Size(
									availableSize.getWidth(),
									Size::unlimited
								)
							);

							const auto& childSize = child->getMeasuredSize();

							if (childSize.getWidth() > result.getWidth())
								result.setWidth(childSize.getWidth());

							result.setHeight(result.getHeight() + childSize.getHeight() + getSpacing());

							visibleChildrenCount++;
						}

						if (visibleChildrenCount > 0)
							result.setHeight(result.getHeight() - getSpacing());

						break;
					}
				}

				return result;
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				int32_t position;

				switch (getOrientation()) {
					case Orientation::horizontal: {
						position = bounds.getX();

						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							const auto& childSize = child->getMeasuredSize();

							child->render(renderer, Bounds(
								position,
								bounds.getY(),
								childSize.getWidth(),
								bounds.getHeight()
							));

							position += childSize.getWidth() + getSpacing();
						}

						break;
					}
					case Orientation::vertical: {
						position = bounds.getY();

						for (auto child: *this) {
							if (!child->isVisible())
								continue;

							const auto& childSize = child->getMeasuredSize();

							child->render(renderer, Bounds(
								bounds.getX(),
								position,
								bounds.getWidth(),
								childSize.getHeight()
							));

							position += childSize.getHeight() + getSpacing();
						}

						break;
					}
				}
			}
	};
}