#pragma once

#include "element.h"
#include "color.h"
#include "rendering/renderer.h"
#include "cmath"
#include "callback.h"
#include "ui/traits/primaryColorElement.h"
#include "ui/traits/secondaryColorElement.h"
#include "event.h"
#include "number.h"

namespace yoba::ui {
	class KnobRotateEvent;

	class Knob : public PrimaryColorElement {
		public:
			void onRender(Renderer* renderer) override;

			void onEvent(InputEvent& event) override;

			float getAngle() const;
			void setAngle(float angle);

			float getAngleStep() const;
			void setAngleStep(float angleStep);

			const Color* getLineColor() const;
			void setLineColor(const Color* lineColor);

			const Color* getMiddleColor() const;
			void setMiddleColor(const Color* value);

			Callback<float, float>& getOnRotate();

		private:
			Callback<float, float> _onRotate {};
			float _angle = 0;
			float _angleStep = radians(10);
			const Color* _lineColor = nullptr;
			const Color* _middleColor = nullptr;

			template<typename Base, typename T>
			inline bool instanceof(const T*) {
				return std::is_base_of<Base, T>::value;
			}
	};
}