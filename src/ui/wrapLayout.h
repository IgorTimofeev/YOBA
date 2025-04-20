#pragma once

#include <unordered_set>
#include <optional>
#include "layout.h"

namespace yoba::ui {
	class WrapLayout : public Layout {
		public:
			uint16_t getHorizontalSpacing() const {
				return _horizontalSpacing;
			}

			void setHorizontalSpacing(uint16_t value) {
				if (value == _horizontalSpacing)
					return;

				_horizontalSpacing = value;

				invalidate();
			}

			uint16_t getVerticalSpacing() const {
				return _verticalSpacing;
			}

			void setVerticalSpacing(uint16_t value) {
				if (value == _verticalSpacing)
					return;

				_verticalSpacing = value;

				invalidate();
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				uint16_t
					width = 0,
					height = 0,
					lineWidthWithSpacing = 0,
					lineHeight = 0;

				for (auto child : *this) {
					child->measure(Size(
						availableSize.getWidth(),
						Size::unlimited
					));

					const auto& childSize = child->getMeasuredSize();

					// Overflow
					if (lineWidthWithSpacing + childSize.getWidth() >= availableSize.getWidth()) {
						if (lineWidthWithSpacing > 0) {
							width = std::max(width, (uint16_t) (lineWidthWithSpacing - _horizontalSpacing));
							height += height > 0 ? _verticalSpacing + lineHeight : lineHeight;
						}

						lineWidthWithSpacing = childSize.getWidth();
						lineHeight = childSize.getHeight();
					}
					// Normal
					else {
						lineWidthWithSpacing += childSize.getWidth() + _horizontalSpacing;

						if (childSize.getHeight() > lineHeight)
							lineHeight = childSize.getHeight();
					}
				}

				// Handling unprocessed line
				if (lineWidthWithSpacing > 0) {
					width = std::max(width, (uint16_t) (lineWidthWithSpacing - _horizontalSpacing));
					height += height > 0 ? _verticalSpacing + lineHeight : lineHeight;
				}

				return { width, height };
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				int32_t x = 0;
				int32_t y = 0;
				uint16_t lineHeight = 0;

				for (auto child : *this) {
					if (!child->isVisible())
						continue;

					const auto& childSize = child->getMeasuredSize();

					// Overflow
					if (x + childSize.getWidth() >= bounds.getWidth()) {
						if (lineHeight > 0)
							y += y > 0 ? _verticalSpacing + lineHeight : lineHeight;

						child->render(renderer, Bounds(
							bounds.getX(),
							bounds.getY() + y,
							childSize.getWidth(),
							childSize.getHeight()
						));

						x = childSize.getWidth() + _horizontalSpacing;
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

						x += childSize.getWidth() + _horizontalSpacing;

						if (childSize.getHeight() > lineHeight)
							lineHeight = childSize.getHeight();
					}
				}
			}

		private:
			uint16_t _horizontalSpacing = 0;
			uint16_t _verticalSpacing = 0;
	};
}