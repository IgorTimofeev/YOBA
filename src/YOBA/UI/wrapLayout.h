#pragma once

#include "layout.h"
#include "YOBA/UI/traits/horizontalAndVerticalSpacingElement.h"

namespace YOBA {
	class WrapLayout : public Layout, public HorizontalAndVerticalSpacingElement {
		protected:
			Size onMeasure(const Size& availableSize) override {
				uint16_t
					width = 0,
					height = 0,
					lineWidthWithSpacing = 0,
					lineHeight = 0;

				for (const auto child : *this) {
					child->measure(Size(
						availableSize.getWidth(),
						Size::unlimited
					));

					const auto& childSize = child->getMeasuredSize();

					// Overflow
					if (lineWidthWithSpacing + childSize.getWidth() > availableSize.getWidth()) {
						if (lineWidthWithSpacing > 0) {
							width = std::max(width, static_cast<uint16_t>(lineWidthWithSpacing - getHorizontalSpacing()));
							height += height > 0 ? getVerticalSpacing() + lineHeight : lineHeight;
						}

						lineWidthWithSpacing = childSize.getWidth();
						lineHeight = childSize.getHeight();
					}
					// Normal
					else {
						lineWidthWithSpacing += childSize.getWidth() + getHorizontalSpacing();

						if (childSize.getHeight() > lineHeight)
							lineHeight = childSize.getHeight();
					}
				}

				// Handling unprocessed line
				if (lineWidthWithSpacing > 0) {
					width = std::max(width, static_cast<uint16_t>(lineWidthWithSpacing - getHorizontalSpacing()));
					height += height > 0 ? getVerticalSpacing() + lineHeight : lineHeight;
				}

				return { width, height };
			}

			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();

				int32_t x = 0;
				int32_t y = 0;
				uint16_t lineHeight = 0;

				for (const auto child : *this) {
					if (!child->isVisible())
						continue;

					const auto& childSize = child->getMeasuredSize();

					// Overflow
					if (x + childSize.getWidth() > bounds.getWidth()) {
						if (lineHeight > 0)
							y += getVerticalSpacing() + lineHeight;

						child->render(renderer, Bounds(
							bounds.getX(),
							bounds.getY() + y,
							childSize.getWidth(),
							childSize.getHeight()
						));

						x = childSize.getWidth() + getHorizontalSpacing();
						lineHeight = childSize.getHeight();
					}
					// Normal
					else {
						child->render(renderer, Bounds(
							bounds.getX() + x,
							bounds.getY() + y,
							childSize.getWidth(),
							childSize.getHeight()
						));

						x += childSize.getWidth() + getHorizontalSpacing();

						if (childSize.getHeight() > lineHeight)
							lineHeight = childSize.getHeight();
					}
				}
			}
	};
}