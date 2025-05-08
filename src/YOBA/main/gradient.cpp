#include "gradient.h"

namespace YOBA {
	GradientStop::GradientStop(float position, const RGB888Color& color) :
		_position(position),
		_color(color)
	{

	}

	float GradientStop::getPosition() const {
		return _position;
	}

	void GradientStop::setPosition(float value) {
		_position = value;
	}

	const RGB888Color& GradientStop::getColor() const {
		return _color;
	}

	void GradientStop::setColor(const RGB888Color& value) {
		_color = RGB888Color(value);
	}

	RGB888Color LinearGradient::getRgb888Color(float position) const {
		if (_stops.size() < 2)
			return {0, 0, 0};

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

		const auto& fromStep = _stops[fromIndex];
		const auto& toStep = _stops[toIndex];

		const auto spaceBetweenSteps = toStep.getPosition() - fromStep.getPosition();
		const auto positionBetweenSteps = position - fromStep.getPosition();

		auto result = fromStep.getColor();
		result.interpolateTo(toStep.getColor(), positionBetweenSteps / spaceBetweenSteps);

		return result;
	}

	std::vector<GradientStop>& LinearGradient::getStops() {
		return _stops;
	}

	void LinearGradient::addRainbowStops() {
		_stops.emplace_back(0, RGB888Color(0xFF, 0x00, 0xFF));
		_stops.emplace_back(0.2, RGB888Color(0x00, 0x00, 0xff));
		_stops.emplace_back(0.4, RGB888Color(0x00, 0xff, 0xff));
		_stops.emplace_back(0.6, RGB888Color(0x00, 0xFF, 0x00));
		_stops.emplace_back(0.8, RGB888Color(0xFF, 0xFF, 0x00));
		_stops.emplace_back(1, RGB888Color(0xFF, 0x00, 0x00));
	}

	void LinearGradient::addRGBStops() {
		_stops.emplace_back(0, RGB888Color(0xFF, 0x00, 0x00));
		_stops.emplace_back(0.5, RGB888Color(0x00, 0xff, 0x00));
		_stops.emplace_back(1, RGB888Color(0x00, 0x00, 0xFF));
	}
}