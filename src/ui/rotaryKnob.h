#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "cmath"
#include "../action.h"
#include "traits/backgroundColorAware.h"
#include "traits/foregroundColorAware.h"
#include "../event.h"
#include "../number.h"

namespace yoba {
	class RotaryKnob : public BackgroundColorAware {
		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				const auto& bounds = getBounds();
				const uint16_t radius = max(bounds.getWidth(), bounds.getHeight()) / 2;
				const auto center = bounds.getCenter();

				// Background circle
				if (getBackgroundColor()) {
					screenBuffer->renderFilledCircle(
						center,
						radius,
						getBackgroundColor()
					);
				}

				// Center circle
				if (getCenterColor()) {
					screenBuffer->renderFilledCircle(
						center,
						(uint16_t) ((float) radius * 0.5f),
						getCenterColor()
					);
				}

				// Line
				if (getLineColor()) {
					const auto lineEnd = center + Point(radius, 0).rotate(_angle - (float) radians(90));
					const auto lineStart = center + (lineEnd - center) * 0.8f;

					screenBuffer->renderLine(
						lineStart,
						lineEnd,
						getLineColor()
					);
				}
			}

			void onEvent(Event &event) override {
				if (event.getType() != EventType::TouchDown && event.getType() != EventType::TouchDrag && event.getType() != EventType::TouchUp)
					return;

				auto& touchEvent = (TouchEvent&) event;

				if (event.getType() == EventType::TouchDown) {
					setCaptured(true);
				}
				else if (event.getType() == EventType::TouchUp) {
					setCaptured(false);
				}
				else if (event.getType() == EventType::TouchDrag) {
					const auto newAngle = touchEvent.getPosition().getRotation(getBounds().getCenter()) - (float) radians(90);
					const auto deltaAngle = newAngle - _angle;

					_angle = newAngle;

					_onRotate.call(deltaAngle);
				}

				event.setHandled(true);
			}

			float getAngle() const {
				return _angle;
			}

			void setAngle(float angle) {
				_angle = angle;
			}

			float getAngleStep() const {
				return _angleStep;
			}

			void setAngleStep(float angleStep) {
				_angleStep = angleStep;
			}

			const Color* getCenterColor() const {
				return _centerColor;
			}

			void setCenterColor(const Color * centerColor) {
				_centerColor = centerColor;
			}

			const Color *getLineColor() const {
				return _lineColor;
			}

			void setLineColor(const Color *lineColor) {
				_lineColor = lineColor;
			}

			void addOnRotate(const std::function<void(float)>& value) {
				_onRotate.add(value);
			}

		private:
			Action<float> _onRotate {};
			float _angle = 0;
			float _angleStep = radians(10);
			const Color* _lineColor = nullptr;
			const Color* _centerColor = nullptr;
	};
}