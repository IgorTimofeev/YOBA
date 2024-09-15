#pragma once

#include "element.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "cmath"
#include "action.h"
#include "background_aware.h"
#include "foreground_aware.h"
#include "../event.h"
#include "../number.h"

namespace yoba {
	class Slider : public BackgroundAware, public ForegroundAware {
		public:
			Slider() {
				setBackground(&Color16::black);
				setForeground(&Color16::white);
			}

			void onEvent(Event &event) override {
				if (event.getType() != EventType::touchDown && event.getType() != EventType::touchDrag && event.getType() != EventType::touchUp)
					return;

				auto touchEvent = (TouchEvent&) event;

				if (event.getType() == EventType::touchDown) {
					setCaptured(true);
				}
				else if (event.getType() == EventType::touchUp) {
					setCaptured(false);
				}

				auto bounds = getBounds();
				auto part = clamp((float) (touchEvent.getPosition().getX() - bounds.getX()) / (float) bounds.getWidth(), 0.0f, 1.0f);

				setValue(part);

				event.setHandled(true);
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				auto bounds = getBounds();
				auto part = (uint16_t) std::round(_value * (float) bounds.getWidth());

				screenBuffer->renderFilledRectangle(
					bounds,
					getBackground()
				);

				screenBuffer->renderFilledRectangle(
					Bounds(
						bounds.getPosition(),
						Size(
							part,
							bounds.getHeight()
						)
					),
					getForeground()
				);
			}

			// -------------------------------- Getters & setters --------------------------------


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
