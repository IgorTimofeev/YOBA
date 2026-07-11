#include <YOBA/UI/Switch.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>

#include "Application.hpp"

namespace YOBA {
	Switch::Switch(const bool isActive) {
		_animation.setDuration(250'000);

		_animation.setOnTick([this] {
			_handlePosition = _handlePositionFrom + ((this->isActive() ? 1.0f : 0.0f) - _handlePositionFrom) * _animation.getProgress();

			invalidateRender();
		});

		setActive(isActive);
	}

	Switch::Switch() : Switch(true) {

	}

	const Color* Switch::getTrackColor() const {
		return _trackColor;
	}

	void Switch::setTrackColor(const Color* value) {
		_trackColor = value;

		invalidateRender();
	}

	const Color* Switch::getCheckedColor() const {
		return _checkedColor;
	}

	void Switch::setCheckedColor(const Color* value) {
		_checkedColor = value;

		invalidateRender();
	}

	const Color* Switch::getHandleColor() const {
		return _handleColor;
	}

	void Switch::setHandleColor(const Color* value) {
		_handleColor = value;

		invalidateRender();
	}

	uint32_t Switch::getAnimationDuration() const {
		return _animation.getDuration();
	}

	void Switch::setAnimationDuration(const uint32_t durationUs) {
		_animation.setDuration(durationUs);
	}

	void Switch::onEvent(Event* event) {
		if (event->getTypeID() != PointerUpEvent::typeID)
			return;

		setActive(!isActive());

		event->setHandled(true);
	}

	void Switch::onIsActiveChanged() {
		if (isVisibleOnTarget()) {
			_animation.stop();

			_handlePositionFrom = _handlePosition;
			_animation.start();
		}
		else {
			_handlePosition = isActive() ? 1.f : 0.f;
			_handlePositionFrom = _handlePosition;
		}
	}

	void Switch::onRender(Renderer* renderer, const Rectangle& bounds) {
		const uint16_t handleWidth = bounds.getHeight();
		const uint16_t handleWidthHalf = handleWidth / 2;

		const auto handleX = static_cast<uint16_t>(
			static_cast<float>(bounds.getWidth() - handleWidth)
			* _handlePosition
		);

		const uint16_t handleXCenter = handleX + handleWidthHalf;

		// Checked
		if (handleX > 0 && _checkedColor) {
			renderer->fillRectangle(
				Rectangle(
					bounds.getX(),
					bounds.getY(),
					handleXCenter + handleWidthHalf,
					bounds.getHeight()
				),
				getCornerRadius(),
				_checkedColor
			);
		}

		// Track
		if (handleX + bounds.getHeight() < bounds.getX2() && _trackColor) {
			renderer->fillRectangle(
				Rectangle(
					bounds.getX() + handleXCenter - handleWidthHalf,
					bounds.getY(),
					bounds.getWidth() - handleXCenter + handleWidthHalf,
					bounds.getHeight()
				),
				getCornerRadius(),
				_trackColor
			);
		}

		// Handle
		if (_handleColor) {
			renderer->fillRectangle(
				Rectangle(
					bounds.getX() + handleX,
					bounds.getY(),
					handleWidth,
					bounds.getHeight()
				),
				getCornerRadius(),
				_handleColor
			);
		}
	}
}
