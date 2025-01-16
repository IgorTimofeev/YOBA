#pragma once

#include "element.h"
#include "color.h"
#include "../rendering/renderer.h"
#include "cmath"
#include "callback.h"
#include "ui/traits/focusableElement.h"
#include "ui/traits/cornerRadiusElement.h"
#include "ui/traits/primaryColorElement.h"
#include "ui/traits/secondaryColorElement.h"
#include "event.h"
#include "number.h"

namespace yoba::ui {
	class Slider : public FocusableElement, public PrimaryColorElement, public SecondaryColorElement, public CornerRadiusElement {
		public:
			Callback<> valueChanged {};

			void onRender(Renderer* renderer) override {
				auto primaryColor = getPrimaryColor();
				auto secondaryColor = getSecondaryColor();

				auto& bounds = getBounds();
				auto part = (uint16_t) std::round(_value * (float) bounds.getWidth());

				if (primaryColor) {
					renderer->renderFilledRectangle(
						bounds,
						getCornerRadius(),
						primaryColor
					);
				}

				renderer->renderFilledRectangle(
					Bounds(
						bounds.getPosition(),
						Size(
							part,
							bounds.getHeight()
						)
					),
					getCornerRadius(),
					secondaryColor
				);
			}

			void onEvent(Event& event) override {
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
				if (value == _value)
					return;

				_value = value;
				clampValue();

				invalidateRender();

				onValueChanged();
				valueChanged();
			}

			virtual void onValueChanged() {

			}

		private:
			float _value = 1;

			void clampValue() {
				_value = clamp(_value, 0.0f, 1.0f);
			}
	};
}
