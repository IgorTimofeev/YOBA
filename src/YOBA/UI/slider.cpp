#include <algorithm>

#include <YOBA/main/math.h>
#include <YOBA/UI/slider.h>
#include <YOBA/main/events/pinchEvent.h>

namespace YOBA {
	const Color* Slider::getTrackColor() const {
		return _trackColor;
	}

	void Slider::setTrackColor(const Color* value) {
		_trackColor = value;

		Element::invalidateRender();
	}

	const Color* Slider::getFillColor() const {
		return _fillColor;
	}

	void Slider::setFillColor(const Color* value) {
		_fillColor = value;

		Element::invalidateRender();
	}

	const Color* Slider::getHandleColor() const {
		return _handleColor;
	}

	void Slider::setHandleColor(const Color* value) {
		_handleColor = value;

		Element::invalidateRender();
	}

	void Slider::setTrackSize(const uint8_t value) {
		_trackSize = value;

		invalidateRender();
	}

	uint8_t Slider::getTrackCornerRadius() const {
		return _trackCornerRadius;
	}

	void Slider::setTrackCornerRadius(const uint8_t value) {
		_trackCornerRadius = value;

		invalidateRender();
	}

	uint8_t Slider::getHandleSize() const {
		return _handleSize;
	}

	void Slider::setHandleSize(const uint8_t value) {
		_handleSize = value;

		invalidateRender();
	}

	uint8_t Slider::getHandleCornerRadius() const {
		return _handleCornerRadius;
	}

	void Slider::setHandleCornerRadius(const uint8_t value) {
		_handleCornerRadius = value;

		invalidateRender();
	}

	void Slider::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto handleHalf = _handleSize / 2;
		const auto trackY = bounds.getY() + handleHalf - _trackSize / 2;
		const auto handleCenterLocal = handleHalf + static_cast<uint16_t>(std::round(getValue() * (bounds.getWidth() - bounds.getHeight()) / valueMax));

		// Fill
		if (getValue() > 0 && _fillColor) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX(),
					trackY,
					handleCenterLocal,
					_trackSize
				),
				_trackCornerRadius,
				_fillColor
			);
		}

		// Track
		if (getValue() < valueMax && _trackColor) {
			renderer->renderFilledRectangle(
				Bounds(
					bounds.getX() + handleCenterLocal,
					trackY,
					bounds.getWidth() - handleCenterLocal,
					_trackSize
				),
				_trackCornerRadius,
				_trackColor
			);
		}

		// Handle
		if (_handleColor) {
			if (_handleSize == bounds.getHeight() && _handleCornerRadius == handleHalf) {
				renderer->renderFilledCircle(
					Point(
						bounds.getX() + handleCenterLocal,
						bounds.getY() + handleHalf
					),
					_handleCornerRadius,
					_handleColor
				);
			}
			else {
				renderer->renderFilledRectangle(
					Bounds(
						bounds.getX() + handleCenterLocal - handleHalf,
						bounds.getY(),
						_handleSize,
						bounds.getHeight()
					),
					_handleCornerRadius,
					_handleColor
				);
			}
		}
	}

	uint8_t Slider::getTrackSize() const {
		return _trackSize;
	}

	void Slider::onEvent(Event* event) {
		if (event->getTypeID() == PointerDownEvent::typeID) {
			setCaptured(true);

			if (isFocused()) {
				updateValueFromEvent(reinterpret_cast<PointerDownEvent*>(event));
			}
			else {
				setFocused(true);
			}

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerDragEvent::typeID) {
			updateValueFromEvent(reinterpret_cast<PointerDragEvent*>(event));

			event->setHandled(true);
		}
		else if (event->getTypeID() == PointerUpEvent::typeID) {
			setCaptured(false);
			event->setHandled(true);
		}
	}

	void Slider::updateValueFromEvent(const PointerEvent* event) {
		const auto& bounds = getBounds();

		const auto localX = std::clamp<int32_t>(
			event->getPosition().getX() - bounds.getX(),
			0,
			bounds.getWidth()
		);

		setValue(localX * valueMax / bounds.getWidth());
	}
}
