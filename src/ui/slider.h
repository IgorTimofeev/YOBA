#pragma once

#include <algorithm>
#include "element.h"
#include "traits/focusableElement.h"
#include "main/event.h"
#include "main/math.h"

namespace yoba::ui {
	class Slider : public FocusableElement {
		public:
			Callback<> valueChanged;

			const Color* getTrackColor() const {
				return _trackColor;
			}

			void setTrackColor(const Color* value) {
				_trackColor = value;

				invalidateRender();
			}

			const Color* getFillColor() const {
				return _fillColor;
			}

			void setFillColor(const Color* value) {
				_fillColor = value;

				invalidateRender();
			}

			const Color* getHandleColor() const {
				return _handleColor;
			}

			void setHandleColor(const Color* value) {
				_handleColor = value;

				invalidateRender();
			}

			uint8_t getTrackSize() const {
				return _trackSize;
			}

			void setTrackSize(uint8_t value) {
				_trackSize = value;

				invalidateRender();
			}

			uint8_t getTrackCornerRadius() const {
				return _trackCornerRadius;
			}

			void setTrackCornerRadius(uint8_t value) {
				_trackCornerRadius = value;

				invalidateRender();
			}

			uint8_t getHandleSize() const {
				return _handleSize;
			}

			void setHandleSize(uint8_t value) {
				_handleSize = value;

				invalidateRender();
			}

			uint8_t getHandleCornerRadius() const {
				return _handleCornerRadius;
			}

			void setHandleCornerRadius(uint8_t value) {
				_handleCornerRadius = value;

				invalidateRender();
			}

			uint16_t getValue() const {
				return _value;
			}

			void setValue(uint16_t value) {
				if (value == _value)
					return;

				_value = value;

				onValueChanged();
				valueChanged();

				invalidateRender();
			}

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				const auto handleHalf = _handleSize / 2;
				const auto trackY = bounds.getY() + handleHalf - _trackSize / 2;
				const auto handleCenterLocal = handleHalf + (uint16_t) std::round(_value * (bounds.getWidth() - bounds.getHeight()) / 0xFFFF);

				// Fill
				if (_value > 0 && _fillColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							trackY,
							handleCenterLocal,
							_trackSize
						),
						_trackCornerRadius,
						_fillColor
					);
				}

				// Track
				if (_value < 0xFFFF && _trackColor) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX() + handleCenterLocal,
							trackY,
							bounds.getWidth() - handleCenterLocal,
							_trackSize
						),
						_trackCornerRadius,
						_trackColor
					);
				}

				// Handle
				if (_handleColor) {
					if (_handleSize == bounds.getHeight() && _handleCornerRadius == handleHalf) {
						renderer->renderFilledCircle(
							Point(
								bounds.getX() + handleCenterLocal,
								bounds.getY() + handleHalf
							),
							_handleCornerRadius,
							_handleColor
						);
					}
					else {
						renderer->renderFilledRectangle(
							Bounds(
								bounds.getX() + handleCenterLocal - handleHalf,
								bounds.getY(),
								_handleSize,
								bounds.getHeight()
							),
							_handleCornerRadius,
							_handleColor
						);
					}
				}
			}

			void onEvent(Event* event) override {
				const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
				const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;
				const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;

				if (!(isTouchDown || isTouchUp || isTouchDrag))
					return;

				if (isTouchDown) {
					setCaptured(true);
					setFocused(true);
				}
				else if (isTouchUp) {
					setCaptured(false);
				}

				const auto& bounds = getBounds();
				const int32_t localX = std::clamp(((TouchEvent*) event)->getPosition().getX() - bounds.getX(), (int32_t) 0, (int32_t) bounds.getWidth());

				setValue(localX * 0xFFFF / bounds.getWidth());

				event->setHandled(true);
			}

			virtual void onValueChanged() {

			}

		private:
			uint8_t _trackSize = 4;
			uint8_t _trackCornerRadius = 2;

			uint8_t _handleSize = 8;
			uint8_t _handleCornerRadius = 4;

			const Color* _trackColor;
			const Color* _fillColor;
			const Color* _handleColor;

			uint16_t _value = 0xFFFF;
	};
}
