#include <YOBA/UI/stackLayout.h>

namespace YOBA {
	StackLayout::StackLayout(const Orientation orientation) {
		setOrientation(orientation);
	}

	StackLayout::StackLayout(const uint16_t spacing) {
		setGap(spacing);
	}

	StackLayout::StackLayout(const Orientation orientation, const uint16_t spacing) {
		setOrientation(orientation);
		setGap(spacing);
	}

	Size StackLayout::onMeasure(const Size& availableSize) {
		auto result = Size();

		size_t visibleChildrenCount = 0;

		switch (getOrientation()) {
			case Orientation::horizontal: {
				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					child->measure(
						Size(
							Size::unlimited,
							availableSize.getHeight()
						)
					);

					const auto& childSize = child->getMeasuredSize();

					result.setWidth(result.getWidth() + childSize.getWidth() + getGap());

					if (childSize.getHeight() > result.getHeight())
						result.setHeight(childSize.getHeight());

					visibleChildrenCount++;
				}

				if (visibleChildrenCount > 0)
					result.setWidth(result.getWidth() - getGap());

				break;
			}
			case Orientation::vertical: {
				for (const auto child: *this) {
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

					result.setHeight(result.getHeight() + childSize.getHeight() + getGap());

					visibleChildrenCount++;
				}

				if (visibleChildrenCount > 0)
					result.setHeight(result.getHeight() - getGap());

				break;
			}
		}

		// ESP_LOGI("Stack lay", "rend height: %f", (float)result.getHeight());

		return result;
	}

	void StackLayout::onRender(Renderer* renderer, const Bounds& bounds) {
		int32_t position;

		switch (getOrientation()) {
			case Orientation::horizontal: {
				position = bounds.getX();

				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					const auto& childSize = child->getMeasuredSize();

					child->render(renderer, Bounds(
			        	position,
			        	bounds.getY(),
			        	childSize.getWidth(),
			        	bounds.getHeight()
		            ));

					position += childSize.getWidth() + getGap();
				}

				break;
			}
			case Orientation::vertical: {
				position = bounds.getY();

				for (const auto child: *this) {
					if (!child->isVisible())
						continue;

					const auto& childSize = child->getMeasuredSize();

					child->render(renderer, Bounds(
						bounds.getX(),
						position,
						bounds.getWidth(),
						childSize.getHeight()
					));

					position += childSize.getHeight() + getGap();
				}

				break;
			}
		}
	}
}
