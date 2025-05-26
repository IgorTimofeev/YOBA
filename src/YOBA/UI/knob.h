#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include "cmath"
#include <YOBA/main/callback.h>
#include <YOBA/UI/traits/backgroundColorElement.h>
#include <YOBA/main/event.h>
#include <YOBA/main/math.h>

namespace YOBA {
	class Knob : public Control, public BackgroundColorElement {
		public:
			Callback<float, float> rotated;

			float getAngle() const;
			void setAngle(float angle);

			float getAngleStep() const;
			void setAngleStep(float angleStep);

			const Color* getLineColor() const;
			void setLineColor(const Color* lineColor);

			const Color* getMiddleColor() const;
			void setMiddleColor(const Color* value);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onTouchDown(TouchDownEvent* event) override;
			void onTouchDrag(TouchDragEvent* event) override;
			void onTouchUp(TouchUpEvent* event) override;

		private:
			float _angle = 0;
			float _angleStep = toRadians(10);
			const Color* _lineColor = nullptr;
			const Color* _middleColor = nullptr;
	};
}