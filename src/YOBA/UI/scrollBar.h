#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/UI/traits/cornerRadiusElement.h"
#include "YOBA/UI/traits/orientationElement.h"

namespace YOBA {
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
			void onRender(Renderer* renderer) override {
				// Thumb
				if (!_thumbColor)
					return;

				const auto& bounds = getBounds();
				const auto size = static_cast<uint16_t>(static_cast<uint64_t>(_viewportSize) * static_cast<uint64_t>(bounds.getHeight()) / static_cast<uint64_t>(_totalSize));
				const auto position = static_cast<uint16_t>(static_cast<uint64_t>(_position) * static_cast<uint64_t>(bounds.getHeight()) / static_cast<uint64_t>(_totalSize));

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