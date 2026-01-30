#include <YOBA/UI/switch.h>
#include <YOBA/main/events/pointerEvent.h>

namespace YOBA {
	Switch::Switch(const bool checked) {
		setActive(checked);
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

	void Switch::onRender(Renderer* renderer, const Bounds& bounds) {
		const uint16_t handleHalf = bounds.getHeight() / 2;
		const uint16_t handleOffset = isActive() ? bounds.getWidth() - bounds.getHeight() : 0;
		const uint16_t handleOffsetCenter = handleOffset + handleHalf;

		// Checked
		if (handleOffset > 0 && _checkedColor) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					bounds.getY(),
					handleOffsetCenter + handleHalf,
					bounds.getHeight()
				),
				getCornerRadius(),
				_checkedColor
			);
		}

		// Track
		if (handleOffset + bounds.getHeight() < bounds.getX2() && _trackColor) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX() + handleOffsetCenter - handleHalf,
					bounds.getY(),
					bounds.getWidth() - handleOffsetCenter + handleHalf,
					bounds.getHeight()
				),
				getCornerRadius(),
				_trackColor
			);
		}

		// Handle
		if (_handleColor) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX() + handleOffset,
					bounds.getY(),
					bounds.getHeight(),
					bounds.getHeight()
				),
				getCornerRadius(),
				_handleColor
			);
		}
	}

	void Switch::onEvent(Event* event) {
		if (event->getTypeID() != PointerUpEvent::typeID)
			return;

		setActive(!isActive());

		event->setHandled(true);
	}

	void Switch::onIsActiveChanged() {
		ActiveElement::onIsActiveChanged();

		isActiveChanged();
	}
}
