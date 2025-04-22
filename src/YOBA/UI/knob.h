#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/color.h"
#include "YOBA/main/rendering/renderer.h"
#include "cmath"
#include "YOBA/main/callback.h"
#include "YOBA/UI/traits/backgroundColorElement.h"
#include "YOBA/main/event.h"
#include "YOBA/main/math.h"

namespace YOBA {
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