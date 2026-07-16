#pragma once

#include <YOBA/UI/Spinner.hpp>
#include <YOBA/System.hpp>
#include <YOBA/Core/Math.hpp>

namespace YOBA {
	void Spinner::renderSpinner(Renderer* renderer, const Rectangle& bounds, const float fromAngleRad, const float arcMeasureRad) const {
		const auto center = bounds.getCenter();
		const auto radius = std::min<int16_t>(bounds.getWidth(), bounds.getHeight()) / 2;

		if (arcMeasureRad < Math::twoPi) {
			renderer->strokeCircle(
				center,
				radius,
				getThickness(),
				getTrackColor()
			);
		}

		if (!getFillColor())
			return;

		if (arcMeasureRad > 0) {
			renderer->strokeArc(
				center,
				radius,
				getThickness(),
				// Inverting CCW -> CW & starting from X = 0, Y = 1
				Math::halfPi - fromAngleRad - arcMeasureRad,
				arcMeasureRad,
				getFillColor()
			);
		}
	}

	void ProgressSpinner::onRender(Renderer* renderer, const Rectangle& bounds) {
		renderSpinner(
			renderer,
			bounds,
			0,
			Math::twoPi * std::min(getValue(), 1.f)
		);
	}

	float IndeterminateSpinner::getArcMeasure() const {
		return _arcMeasureRad;
	}

	void IndeterminateSpinner::setArcMeasure(const float arcMeasureRad) {
		_arcMeasureRad = arcMeasureRad;
	}

	float IndeterminateSpinner::getRotationSpeed() const {
		return _rotationSpeedRadPerSecond;
	}

	void IndeterminateSpinner::setRotationSpeed(const float radPerSecond) {
		_rotationSpeedRadPerSecond = radPerSecond;
	}

	void IndeterminateSpinner::onTick() {
		// speedRad = 1S
		// xRad - deltaS
		_fromAngleRad = _fromAngleRad + static_cast<float>(system::getTimeUs() - _tickTime) * _rotationSpeedRadPerSecond / 1'000'000.f;

		_tickTime = system::getTimeUs();
	}

	void IndeterminateSpinner::onRender(Renderer* renderer, const Rectangle& bounds) {
		renderSpinner(
			renderer,
			bounds,
			_fromAngleRad,
			_arcMeasureRad
		);
	}
}
