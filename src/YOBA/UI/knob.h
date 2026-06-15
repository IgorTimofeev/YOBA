#pragma once

#include <cmath>
#include <functional>

#include <YOBA/UI/control.h>
#include <YOBA/core/color.h>
#include <YOBA/rendering/renderer.h>
#include <YOBA/UI/traits/backgroundColorElement.h>
#include <YOBA/core/events/pointerEvent.h>
#include <YOBA/core/math.h>

namespace YOBA {
	class Knob : public Control, public BackgroundColorElement {
		public:
			float getAngle() const;
			void setAngle(float angle);

			float getAngleStep() const;
			void setAngleStep(float angleStep);

			const Color* getLineColor() const;
			void setLineColor(const Color* lineColor);

			const Color* getMiddleColor() const;
			void setMiddleColor(const Color* value);

			void setOnRotate(const std::function<void(float, float)>& onRotate);

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override;
			void onEvent(Event* event) override;

		private:
			float _angle = 0;
			float _angleStep = toRadians(10);
			const Color* _lineColor = nullptr;
			const Color* _middleColor = nullptr;
			std::function<void(float, float)> _onRotate = nullptr;
	};
}