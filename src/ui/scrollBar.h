#pragma once

#include "element.h"
#include "traits/cornerRadiusElement.h"
#include "traits/primaryColorElement.h"
#include "traits/orientationElement.h"

namespace yoba::ui {
	class ScrollBar : public OrientationElement, public CornerRadiusElement {
		public:
			uint16_t getPosition() const {
				return _position;
			}

			void setPosition(uint16_t value) {
				_position = value;

				invalidate();
			}

			uint16_t getViewportSize() const {
				return _viewportSize;
			}

			void setViewportSize(uint16_t value) {
				_viewportSize = value;

				invalidate();
			}

			uint16_t getTotalSize() const {
				return _totalSize;
			}

			void setTotalSize(uint16_t value) {
				_totalSize = value;

				invalidate();
			}

			const Color* getThumbColor() const {
				return _thumbColor;
			}

			void setThumbColor(const Color* value) {
				_thumbColor = value;

				invalidate();
			}

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				// Thumb
				if (!_thumbColor)
					return;

				const auto size = (uint16_t) ((uint64_t) _viewportSize * (uint64_t) bounds.getHeight() / (uint64_t) _totalSize);
				const auto position = (uint16_t) ((uint64_t) _position * (uint64_t) bounds.getHeight() / (uint64_t) _totalSize);

				if (getOrientation() == Orientation::horizontal) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + position,
							bounds.getY(),
							size,
							bounds.getHeight()
						),
						getCornerRadius(),
						_thumbColor
					);
				}
				else {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							bounds.getY() + position,
							bounds.getWidth(),
							size
						),
						getCornerRadius(),
						_thumbColor
					);
				}
			}

		private:
			uint16_t _position = 0;
			uint16_t _viewportSize = 0;
			uint16_t _totalSize = 0;

			const Color* _thumbColor = nullptr;
	};
}