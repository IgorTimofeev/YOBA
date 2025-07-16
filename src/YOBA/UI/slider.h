#pragma once

#include <algorithm>
#include <YOBA/UI/control.h>
#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/main/events/pinchEvent.h>
#include <YOBA/main/math.h>

namespace YOBA {
	class Slider : public Control {
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

			void setTrackSize(const uint8_t value) {
				_trackSize = value;

				invalidateRender();
			}

			uint8_t getTrackCornerRadius() const {
				return _trackCornerRadius;
			}

			void setTrackCornerRadius(const uint8_t value) {
				_trackCornerRadius = value;

				invalidateRender();
			}

			uint8_t getHandleSize() const {
				return _handleSize;
			}

			void setHandleSize(const uint8_t value) {
				_handleSize = value;

				invalidateRender();
			}

			uint8_t getHandleCornerRadius() const {
				return _handleCornerRadius;
			}

			void setHandleCornerRadius(const uint8_t value) {
				_handleCornerRadius = value;

				invalidateRender();
			}

			uint16_t getValue() const {
				return _value;
			}

			void setValue(const uint16_t value) {
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
				const auto handleCenterLocal = handleHalf + static_cast<uint16_t>(std::round(_value * (bounds.getWidth() - bounds.getHeight()) / 0xFFFF));

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
				if (event->getTypeID() == PointerDownEvent::typeID) {
					setCaptured(true);
					setFocused(true);

					event->setHandled(true);
				}
				else if (event->getTypeID() == PointerDragEvent::typeID) {
					const auto& bounds = getBounds();
					const int32_t localX = std::clamp(reinterpret_cast<PointerDragEvent*>(event)->getPosition().getX() - bounds.getX(), static_cast<int32_t>(0), static_cast<int32_t>(bounds.getWidth()));

					setValue(localX * 0xFFFF / bounds.getWidth());

					event->setHandled(true);
				}
				else if (event->getTypeID() == PointerUpEvent::typeID) {
					setCaptured(false);
					event->setHandled(true);
				}
			}

			virtual void onValueChanged() {

			}

		private:
			uint8_t _trackSize = 4;
			uint8_t _trackCornerRadius = 2;

			uint8_t _handleSize = 8;
			uint8_t _handleCornerRadius = 4;

			const Color* _trackColor = nullptr;
			const Color* _fillColor = nullptr;
			const Color* _handleColor = nullptr;

			uint16_t _value = 0xFFFF;
	};
}
