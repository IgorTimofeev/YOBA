#pragma once

#include "color.h"
#include "vector"
#include <cmath>

namespace YOBA {
	class GradientStop {
		public:
			GradientStop(float position, const RGB888Color& color);

			float getPosition() const;

			void setPosition(float value);

			const RGB888Color& getColor() const;

			void setColor(const RGB888Color& value);

		private:
			float position;
			RGB888Color color;
	};

	class LinearGradient {
		public:
			RGB888Color getRgb888Color(float position) const;

			std::vector<GradientStop>& getStops();

			void addRainbowStops();

			void addRGBStops();

		private:
			std::vector<GradientStop> _stops;
	};
}