#pragma once

#include "element.h"
#include "main/color.h"
#include "rendering/renderers/renderer.h"
#include "cmath"
#include "main/callback.h"
#include "ui/traits/backgroundColorElement.h"
#include "main/event.h"
#include "main/math.h"

namespace yoba::ui {
	class KnobRotateEvent;

	class Knob : public BackgroundColorElement {
		public:
			Callback<float, float> rotated;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onEvent(Event* event) override;

			float getAngle() const;
			void setAngle(float angle);

			float getAngleStep() const;
			void setAngleStep(float angleStep);

			const Color* getLineColor() const;
			void setLineColor(const Color* lineColor);

			const Color* getMiddleColor() const;
			void setMiddleColor(const Color* value);

		private:
			float _angle = 0;
			float _angleStep = toRadians(10);
			const Color* _lineColor = nullptr;
			const Color* _middleColor = nullptr;

			template<typename Base, typename T>
			inline bool instanceof(const T*) {
				return std::is_base_of<Base, T>::value;
			}
	};
}