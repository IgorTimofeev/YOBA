#pragma once

#include "color.h"
#include "vector"
#include <cmath>

namespace yoba {
	class GradientStop {
		public:
			GradientStop(float position, TrueColor* color) :
				_position(position),
				_color(color)
			{

			}

			float getPosition() const {
				return _position;
			}

			void setPosition(float value) {
				_position = value;
			}

			TrueColor* getColor() {
				return _color;
			}

			void setColor(TrueColor* value) {
				_color = value;
			}

		private:
			float _position;
			TrueColor* _color;
	};

	class LinearGradient {
		public:
			TrueColor* getTrueColor(float position) {
				if (_stops.size() < 2)
					return &TrueColor::black;

				// |------|----|--|---|
				//               *
				size_t fromIndex = 0;
				size_t sideIndex = 0;
				float minDelta = 1;
				float delta;

				for (size_t i = 0; i < _stops.size(); i++) {
					if (_stops[i].getPosition() < _stops[sideIndex].getPosition())
						sideIndex = i;

					if (position < _stops[i].getPosition())
						continue;

					delta = position - _stops[i].getPosition();

					if (delta < minDelta) {
						minDelta = delta;
						fromIndex = i;
					}
				}

				if (position < _stops[sideIndex].getPosition())
					return _stops[sideIndex].getColor();

				size_t toIndex = fromIndex;
				sideIndex = _stops.size() -1;
				minDelta = 1;

				for (size_t i = 0; i < _stops.size(); i++) {
					if (_stops[i].getPosition() > _stops[sideIndex].getPosition())
						sideIndex = i;

					if (_stops[i].getPosition() <= _stops[fromIndex].getPosition())
						continue;

					delta = _stops[i].getPosition() - _stops[fromIndex].getPosition();

					if (delta < minDelta) {
						minDelta = delta;
						toIndex = i;
					}
				}

				if (position > _stops[sideIndex].getPosition())
					return _stops[sideIndex].getColor();

				auto& fromStep = _stops[fromIndex];
				auto& toStep = _stops[toIndex];

				auto spaceBetweenSteps = toStep.getPosition() - fromStep.getPosition();
				auto positionBetweenSteps = position - fromStep.getPosition();

				auto result = fromStep.getColor();
				result.interpolateTo(toStep.getColor(), positionBetweenSteps / spaceBetweenSteps);

				return result;
			}

			std::vector<GradientStop>& getStops() {
				return _stops;
			}

			void addRainbowStops() {
				_stops.emplace_back(0, TrueColor(0xFF, 0x00, 0xFF));
				_stops.emplace_back(0.2, TrueColor(0x00, 0x00, 0xff));
				_stops.emplace_back(0.4, TrueColor(0x00, 0xff, 0xff));
				_stops.emplace_back(0.6, TrueColor(0x00, 0xFF, 0x00));
				_stops.emplace_back(0.8, TrueColor(0xFF, 0xFF, 0x00));
				_stops.emplace_back(1, TrueColor(0xFF, 0x00, 0x00));
			}

			void addRGBStops() {
				_stops.emplace_back(0, TrueColor(0xFF, 0x00, 0x00));
				_stops.emplace_back(0.5, TrueColor(0x00, 0xff, 0x00));
				_stops.emplace_back(1, TrueColor(0x00, 0x00, 0xFF));
			}

		private:
			std::vector<GradientStop> _stops {};
	};
}