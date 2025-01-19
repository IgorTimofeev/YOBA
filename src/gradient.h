#pragma once

#include "color.h"
#include "vector"
#include <cmath>

namespace yoba {
	class GradientStop {
		public:
			GradientStop(float position, const Rgb888Color& color);

			float getPosition() const;

			void setPosition(float value);

			const Rgb888Color& getColor() const;

			void setColor(const Rgb888Color& value);

		private:
			float _position;
			Rgb888Color _color;
	};

	class LinearGradient {
		public:
			Rgb888Color getRgb888Color(float position);

			std::vector<GradientStop>& getStops();

			void addRainbowStops();

			void addRGBStops();

		private:
			std::vector<GradientStop> _stops;
	};
}