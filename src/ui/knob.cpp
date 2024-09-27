#include "knob.h"

namespace yoba {
	void Knob::onRender(ScreenBuffer* screenBuffer) {
		const auto& bounds = getBounds();
		const uint16_t radius = max(bounds.getWidth(), bounds.getHeight()) / 2;
		const auto center = bounds.getCenter();

		// Background circle
		if (getBackgroundColor()) {
			screenBuffer->renderFilledCircle(
				center,
				radius,
				getBackgroundColor()
			);
		}

		// Center circle
		if (getCenterColor()) {
			screenBuffer->renderFilledCircle(
				center,
				(uint16_t) ((float) radius * 0.5f),
				getCenterColor()
			);
		}

		// Line
		if (getLineColor()) {
			auto centerF = (Vector2F) center;
			const auto lineEnd = centerF + Vector2F(radius, 0).rotate(_angle - (float) radians(90));
			const auto lineStart = centerF + (lineEnd - centerF) * 0.8f;

			screenBuffer->renderLine(
				(Point) lineStart,
				(Point) lineEnd,
				getLineColor()
			);
		}
	}

	void Knob::onEvent(InputEvent &event) {
		if (event.getType() != EventType::TouchDown && event.getType() != EventType::TouchDrag && event.getType() != EventType::TouchUp)
			return;

		auto& touchEvent = (TouchEvent&) event;

		if (event.getType() == EventType::TouchDown) {
			setCaptured(true);
		}
		else if (event.getType() == EventType::TouchUp) {
			setCaptured(false);
		}
		else if (event.getType() == EventType::TouchDrag) {
			const auto oldAngle = _angle;
			_angle = touchEvent.getPosition().getRotation(getBounds().getCenter()) - (float) radians(90);

			auto knobRotateEvent = KnobRotateEvent(this, oldAngle, _angle);
			_onRotate.call(knobRotateEvent);
		}

		event.setHandled(true);
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

	const Color *Knob::getLineColor() const {
		return _lineColor;
	}

	void Knob::setLineColor(const Color *lineColor) {
		_lineColor = lineColor;
	}

	const Color *Knob::getCenterColor() const {
		return _centerColor;
	}

	void Knob::setCenterColor(const Color *centerColor) {
		_centerColor = centerColor;
	}

	Action<KnobRotateEvent&> &Knob::getOnRotate() {
		return _onRotate;
	}

	KnobRotateEvent::KnobRotateEvent(Knob* target, float oldAngle, float newAngle) : TargetEvent(target), _oldAngle(oldAngle), _newAngle(newAngle) {}

	float KnobRotateEvent::getOldAngle() const {
		return _oldAngle;
	}

	float KnobRotateEvent::getNewAngle() const {
		return _newAngle;
	}

	float KnobRotateEvent::getDeltaAngle() const {
		return _newAngle - _oldAngle;
	}
}