#pragma once

#include "element.h"
#include "../color.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "cmath"
#include "../callback.h"
#include "focusableElement.h"
#include "cornerRadiusElement.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "../event.h"
#include "../number.h"

namespace yoba {
	class Slider : public FocusableElement, public PrimaryColorElement, public SecondaryColorElement, public CornerRadiusElement {
		public:
			void onRender(ScreenBuffer* screenBuffer) override {
				auto primaryColor = getPrimaryColor();

				if (!primaryColor)
					primaryColor = screenBuffer->getPrimaryColor();

				auto secondaryColor = getSecondaryColor();

				if (!secondaryColor)
					secondaryColor = screenBuffer->getSecondaryColor();

				auto& bounds = getBounds();
				auto part = (uint16_t) std::round(_value * (float) bounds.getWidth());

				screenBuffer->renderFilledRectangle(
					bounds,
					getCornerRadius(),
					primaryColor
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
					secondaryColor
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
					setFocused(true);
				}
				else if (isTouchUp) {
					setCaptured(false);
					setFocused(false);
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

				_onValueChanged();
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
