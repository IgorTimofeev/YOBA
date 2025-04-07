#include "knob.h"

namespace yoba::ui {
	void Knob::onRender(Renderer* renderer, const Bounds& bounds) {
		const uint16_t radius = std::max(bounds.getWidth(), bounds.getHeight()) / 2;
		const auto center = bounds.getCenter();

		// Primary circle
		if (getBackgroundColor()) {
			renderer->renderFilledCircle(
				center,
				radius,
				getBackgroundColor()
			);
		}

		// Center circle
		if (getMiddleColor()) {
			renderer->renderFilledCircle(
				center,
				(uint16_t) ((float) radius * 0.5f),
				getMiddleColor()
			);
		}

		// Line
		if (getLineColor()) {
			auto centerF = (Vector2F) center;
			const auto lineEnd = centerF + Vector2F(radius, 0).rotate(_angle - (float) toRadians(90));
			const auto lineStart = centerF + (lineEnd - centerF) * 0.8f;

			renderer->renderLine(
				(Point) lineStart,
				(Point) lineEnd,
				getLineColor()
			);
		}
	}

	void Knob::onEvent(Event* event) {
		const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
		const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;
		const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;

		if (!(isTouchDown || isTouchUp || isTouchDrag))
			return;

		if (isTouchDown) {
			setCaptured(true);
		}
		else if (isTouchUp) {
			setCaptured(false);
		}
		else {
			const auto oldAngle = _angle;
			_angle = ((TouchDragEvent*) event)->getPosition().getRotationFloat(getBounds().getCenter()) - (float) toRadians(90);

			rotated(oldAngle, _angle);
		}

		event->setHandled(true);
	}

	float Knob::getAngle() const {
		return _angle;
	}

	void Knob::setAngle(float angle) {
		_angle = angle;
	}

	float Knob::getAngleStep() const {
		return _angleStep;
	}

	void Knob::setAngleStep(float angleStep) {
		_angleStep = angleStep;
	}

	const Color* Knob::getLineColor() const {
		return _lineColor;
	}

	void Knob::setLineColor(const Color* lineColor) {
		_lineColor = lineColor;
	}

	const Color* Knob::getMiddleColor() const {
		return _middleColor;
	}

	void Knob::setMiddleColor(const Color* value) {
		_middleColor = value;
	}
}