#pragma once

#include "element.h"
#include "ui/traits/cornerRadiusElement.h"
#include "main/event.h"
#include "main/math.h"

namespace yoba::ui {
	class ProgressBar : public CornerRadiusElement {
		public:
			Callback<> valueChanged;

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
			void onRender(Renderer* renderer, const Bounds& bounds) override {
				// Track
				if (_trackColor && _value < 0xFFFF) {
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
							_value * bounds.getWidth() / 0xFFFF,
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

			uint16_t _value = 0xFFFF;
	};
}
