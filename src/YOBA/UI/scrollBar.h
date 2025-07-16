#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>
#include <YOBA/UI/traits/orientationElement.h>

namespace YOBA {
	class ScrollBar : public Control, public OrientationElement, public CornerRadiusElement {
		public:
			uint16_t getPosition() const {
				return position;
			}

			void setPosition(const uint16_t value) {
				position = value;

				invalidate();
			}

			uint16_t getViewportSize() const {
				return viewportSize;
			}

			void setViewportSize(const uint16_t value) {
				viewportSize = value;

				invalidate();
			}

			uint16_t getContentSize() const {
				return contentSize;
			}

			void setContentSize(const uint16_t value) {
				contentSize = value;

				invalidate();
			}

			const Color* getThumbColor() const {
				return thumbColor;
			}

			void setThumbColor(const Color* value) {
				thumbColor = value;

				invalidate();
			}

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				// Thumb
				if (!thumbColor)
					return;

				const auto size = static_cast<uint16_t>(static_cast<uint64_t>(bounds.getHeight()) * static_cast<uint64_t>(viewportSize) / static_cast<uint64_t>(contentSize));
				const auto renderPos = static_cast<uint16_t>(static_cast<uint64_t>(bounds.getHeight()) * static_cast<uint64_t>(position) / static_cast<uint64_t>(contentSize));

				if (getOrientation() == Orientation::horizontal) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + renderPos,
							bounds.getY(),
							size,
							bounds.getHeight()
						),
						getCornerRadius(),
						thumbColor
					);
				}
				else {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							bounds.getY() + renderPos,
							bounds.getWidth(),
							size
						),
						getCornerRadius(),
						thumbColor
					);
				}
			}

		private:
			uint16_t position = 0;
			uint16_t viewportSize = 0;
			uint16_t contentSize = 0;

			const Color* thumbColor = nullptr;
	};
}