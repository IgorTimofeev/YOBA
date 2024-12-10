#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/generic/screenBuffer.h"
#include "cmath"
#include "../callback.h"
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
						bounds.getTopLeft(),
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
				const auto isTouchDown = event.getTypeID() == TouchDownEvent::typeID;
				const auto isTouchUp = event.getTypeID() == TouchUpEvent::typeID;
				const auto isTouchDrag = event.getTypeID() == TouchDragEvent::typeID;

				if (!(isTouchDown || isTouchUp || isTouchDrag))
					return;

				if (isTouchDown) {
					setCaptured(true);
				}
				else if (isTouchUp) {
					setCaptured(false);
				}

				const auto& bounds = getBounds();
				const auto part = clamp((float) (((TouchEvent&) event).getPosition().getX() - bounds.getX()) / (float) bounds.getWidth(), 0.0f, 1.0f);

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

			const Callback<>& getOnValueChanged() const {
				return _onValueChanged;
			}

		private:
			float _value = 1;

			Callback<> _onValueChanged {};

			void clampValue() {
				_value = clamp(_value, 0.0f, 1.0f);
			}
	};
}
