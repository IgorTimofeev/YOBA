#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "cmath"
#include "../action.h"
#include "traits/cornerRadiusAware.h"
#include "traits/backgroundColorAware.h"
#include "traits/foregroundColorAware.h"
#include "../event.h"
#include "../number.h"

namespace yoba {
	class Slider : public BackgroundColorAware, public ForegroundColorAware, public CornerRadiusAware {
		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				if (!getBackgroundColor() || !getForegroundColor())
					return;

				auto& bounds = getBounds();
				auto part = (uint16_t) std::round(_value * (float) bounds.getWidth());

				screenBuffer->renderFilledRectangle(
					bounds,
					getCornerRadius(),
					getBackgroundColor()
				);

				screenBuffer->renderFilledRectangle(
					Bounds(
						bounds.getPosition(),
						Size(
							part,
							bounds.getHeight()
						)
					),
					getCornerRadius(),
					getForegroundColor()
				);
			}

			void onEvent(InputEvent &event) override {
				if (event.getType() != InputEventType::TouchDown && event.getType() != InputEventType::TouchDrag && event.getType() != InputEventType::TouchUp)
					return;

				auto& touchEvent = (TouchEvent&) event;

				if (event.getType() == InputEventType::TouchDown) {
					setCaptured(true);
				}
				else if (event.getType() == InputEventType::TouchUp) {
					setCaptured(false);
				}

				auto& bounds = getBounds();
				auto part = clamp((float) (touchEvent.getPosition().getX() - bounds.getX()) / (float) bounds.getWidth(), 0.0f, 1.0f);

				setValue(part);

				event.setHandled(true);
			}
			float getValue() const {
				return _value;
			}

			void setValue(float value) {
				_value = value;

				clampValue();
				invalidateRender();

				_onValueChanged.call();
			}

			virtual void addOnValueChanged(const std::function<void()>& handler) {
				_onValueChanged.add(handler);
			}

		private:
			float _value = 1;

			Action<> _onValueChanged {};

			void clampValue() {
				_value = clamp(_value, 0.0f, 1.0f);
			}
	};
}
