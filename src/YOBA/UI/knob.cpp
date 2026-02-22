#include "knob.h"

namespace YOBA {
	void Knob::onRender(Renderer& renderer, const Bounds& bounds) {
		const uint16_t radius = std::max(bounds.getWidth(), bounds.getHeight()) / 2;
		const auto center = bounds.getCenter();

		// Primary circle
		if (getBackgroundColor()) {
			renderer.renderFilledCircle(
				center,
				radius,
				getBackgroundColor()
			);
		}

		// Center circle
		if (getMiddleColor()) {
			renderer.renderFilledCircle(
				center,
				static_cast<uint16_t>(static_cast<float>(radius) * 0.5f),
				getMiddleColor()
			);
		}

		// Line
		if (getLineColor()) {
			const auto centerF = static_cast<Vector2F>(center);
			const auto lineEnd = centerF + Vector2F(radius, 0).rotate(_angle - toRadians(90));
			const auto lineStart = centerF + (lineEnd - centerF) * 0.8f;

			renderer.renderLine(
				static_cast<Point>(lineStart),
				static_cast<Point>(lineEnd),
				getLineColor()
			);
		}
	}

	void Knob::onEvent(Event& event) {
		if (event.getTypeID() == PointerDownEvent::typeID) {
			setCaptured(true);

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerDragEvent::typeID) {
			const auto oldAngle = _angle;
			_angle = reinterpret_cast<PointerDragEvent&>(event).getPosition().getRotationFloat(getBounds().getCenter()) - toRadians(90);

			if (_onRotate)
				_onRotate(oldAngle, _angle);

			event.setHandled(true);
		}
		else if (event.getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);

			event.setHandled(true);
		}
	}

	float Knob::getAngle() const {
		return _angle;
	}

	void Knob::setAngle(const float angle) {
		_angle = angle;
	}

	float Knob::getAngleStep() const {
		return _angleStep;
	}

	void Knob::setAngleStep(const float angleStep) {
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

	void Knob::setOnRotate(const std::function<void(float, float)>& onRotate) {
		_onRotate = onRotate;
	}
}
