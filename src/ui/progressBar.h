#pragma once

#include "element.h"
#include "ui/traits/cornerRadiusElement.h"
#include "main/event.h"
#include "main/math.h"

namespace yoba::ui {
	class ProgressBar : public CornerRadiusElement {
		public:
			Callback<> valueChanged;

			float getValue() const {
				return _value;
			}

			void setValue(float value) {
				if (value == _value)
					return;

				_value = value;

				onValueChanged();
				valueChanged();

				invalidateRender();
			}

			const Color* getTrackColor() const {
				return _trackColor;
			}

			void setTrackColor(const Color* value) {
				_trackColor = value;

				invalidate();
			}

			const Color* getFillColor() const {
				return _fillColor;
			}

			void setFillColor(const Color* value) {
				_fillColor = value;

				invalidate();
			}

		protected:
			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();

				// Track
				if (_trackColor && _value < 1) {
					renderer->renderFilledRectangle(
						bounds,
						getCornerRadius(),
						_trackColor
					);
				}

				// Fill
				if (_fillColor && _value > 0) {
					renderer->renderFilledRectangle(
						Bounds(
							bounds.getX(),
							bounds.getY(),
							(uint16_t) std::round(_value * (float) bounds.getWidth()),
							bounds.getHeight()
						),
						getCornerRadius(),
						_fillColor
					);
				}
			}

			virtual void onValueChanged() {

			}

		private:
			const Color* _trackColor;
			const Color* _fillColor;

			float _value = 1;
	};
}
