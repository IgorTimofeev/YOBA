#pragma once

#include "element.h"
#include "../color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "cmath"
#include "../callback.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "../event.h"
#include "../number.h"

namespace yoba {
	class KnobRotateEvent;

	class Knob : public PrimaryColorElement {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent &event) override;

			float getAngle() const;
			void setAngle(float angle);

			float getAngleStep() const;
			void setAngleStep(float angleStep);

			const Color *getLineColor() const;
			void setLineColor(const Color *lineColor);

			const Color *getMiddleColor() const;
			void setMiddleColor(const Color *value);

			Callback<KnobRotateEvent&>& getOnRotate();

		private:
			Callback<KnobRotateEvent&> _onRotate {};
			float _angle = 0;
			float _angleStep = radians(10);
			const Color* _lineColor = nullptr;
			const Color* _middleColor = nullptr;

			template<typename Base, typename T>
			inline bool instanceof(const T*) {
				return std::is_base_of<Base, T>::value;
			}
	};

	class KnobRotateEvent : public TargetEvent<Knob*> {
		public:
			KnobRotateEvent(Knob* target, float oldAngle, float newAngle);

			float getOldAngle() const;
			float getNewAngle() const;
			float getDeltaAngle() const;

		private:
			float _oldAngle;
			float _newAngle;
	};
}