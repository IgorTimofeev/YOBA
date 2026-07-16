#pragma once

#include <algorithm>

#include <YOBA/Core/Math.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Traits/ValueElement.hpp>
#include <YOBA/UI/Traits/TrackColorElement.hpp>
#include <YOBA/UI/Traits/FillColorElement.hpp>
#include <YOBA/UI/Traits/ThicknessElement.hpp>

namespace YOBA {
	class Spinner : public Control, public TrackColorElement, public FillColorElement, public ThicknessElement {
		protected:
			void renderSpinner(Renderer* renderer, const Rectangle& bounds, const float fromAngleRad, const float arcMeasureRad) const;
	};

	class ProgressSpinner : public Spinner, public ValueElement<float> {
		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override;
	};

	class IndeterminateSpinner : public Spinner {
		public:
			float getArcMeasure() const;
			void setArcMeasure(const float arcMeasureRad);

			float getRotationSpeed() const;
			void setRotationSpeed(const float radPerSecond);

		protected:
			void onTick() override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			float _fromAngleRad = 0;
			float _arcMeasureRad = Math::toRadians(90);
			float _rotationSpeedRadPerSecond = Math::toRadians(360);

			int64_t _tickTime = 0;
	};
}
