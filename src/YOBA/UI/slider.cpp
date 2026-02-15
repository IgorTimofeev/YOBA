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

	const Size& Slider::getHandleSize() const {
		return _handleSize;
	}

	void Slider::setHandleSize(const Size& value) {
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

	float Slider::getMinimumValue() const {
		return _minimumValue;
	}

	void Slider::setMinimumValue(const float value) {
		_minimumValue = value;
	}

	float Slider::getMaximumValue() const {
		return _maximumValue;
	}

	void Slider::setMaximumValue(const float value) {
		_maximumValue = value;
	}

	void Slider::setValue(const float value) {
		ValueElement::setValue(std::clamp(value, _minimumValue, _maximumValue));
	}

	float Slider::getValueFactor() const {
		return (getValue() - _minimumValue) / (_maximumValue - _minimumValue);
	}

	uint8_t Slider::getTickCount() const {
		return _tickCount;
	}

	void Slider::setTickCount(const uint8_t value) {
		_tickCount = value;
	}

	const Color* Slider::getTickColor() const {
		return _tickColor;
	}

	void Slider::setTickColor(const Color* value) {
		_tickColor = value;
	}

	uint8_t Slider::getSmallTickLineLength() const {
		return _smallTickLineLength;
	}

	void Slider::setSmallTickLineLength(const uint8_t value) {
		_smallTickLineLength = value;
	}

	uint8_t Slider::getBigTickLineLength() const {
		return _bigTickLineLength;
	}

	void Slider::setBigTickLineLength(const uint8_t value) {
		_bigTickLineLength = value;
	}

	uint8_t Slider::getTickOffset() const {
		return _tickOffset;
	}

	void Slider::setTickOffset(const uint8_t value) {
		_tickOffset = value;
	}

	uint8_t Slider::getTickLabelOffset() const {
		return _tickLabelOffset;
	}

	void Slider::setTickLabelOffset(const uint8_t value) {
		_tickLabelOffset = value;
	}

	uint8_t Slider::getBigTickStep() const {
		return _bigTickStep;
	}

	void Slider::setBigTickStep(const uint8_t value) {
		_bigTickStep = value;
	}

	const Font* Slider::getTickLabelFont() const {
		return _tickLabelFont;
	}

	void Slider::setTickLabelFont(const Font* value) {
		_tickLabelFont = value;

		invalidate();
	}

	const std::function<std::wstring_view(float)>& Slider::getTickLabelBuilder() const {
		return _tickLabelBuilder;
	}

	void Slider::setTickLabelBuilder(const std::function<std::wstring_view(float)>& value) {
		_tickLabelBuilder = value;
	}

	Size Slider::onMeasure(const Size& availableSize) {
		uint16_t height = _handleSize.getHeight() / 2 + _trackSize / 2;

		if (_tickCount > 0) {
			height += _tickOffset + _bigTickLineLength;

			if (_tickLabelFont) {
				height += _tickLabelOffset + _tickLabelFont->getHeight();
			}
		}
		else {
			height += _handleSize.getHeight() / 2;
		}

		return {
			availableSize.getWidth(),
			height
		};
	}

	void Slider::onRender(Renderer* renderer, const Bounds& bounds) {
		const auto handleWidthHalf = _handleSize.getWidth() / 2;
		const auto handleHeightHalf = _handleSize.getHeight() / 2;

		const auto trackY = bounds.getY() + handleHeightHalf - _trackSize / 2;
		const auto valueFactor = getValueFactor();

		const auto handleCenterLocal =
			handleWidthHalf
			+ static_cast<uint16_t>(valueFactor * static_cast<float>(bounds.getWidth() - _handleSize.getWidth()));

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
		if (_trackColor) {
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

		// Ticks
		if (_tickCount > 0 && _tickColor) {
			const auto tickY = trackY + _trackSize + _tickOffset;
			float tickXF = bounds.getX() + handleWidthHalf;
			float tickValue = _minimumValue;
			const auto tickValueInterval = (_maximumValue - _minimumValue) / _tickCount;
			const auto tickPixelInterval = static_cast<float>(bounds.getWidth() - handleWidthHalf) / _tickCount;

			for (uint16_t tickIndex = 0; tickIndex < _tickCount + 1; tickIndex++) {
				const auto isBig = tickIndex % _bigTickStep == 0;
				const auto lineLength = isBig ? _bigTickLineLength : _smallTickLineLength;

				// Line
				renderer->renderVerticalLine(
					Point(static_cast<int32_t>(tickXF), tickY),
					lineLength,
					_tickColor
				);

				// Text
				if (isBig && _tickLabelFont) {
					const auto text = std::wstring(_tickLabelBuilder(tickValue));

					int32_t textX;

					if (tickIndex == 0) {
						textX = static_cast<int32_t>(tickXF);
					}
					else if (tickIndex < _tickCount) {
						textX = static_cast<int32_t>(tickXF - static_cast<float>(_tickLabelFont->getWidth(text)) / 2);
					}
					else {
						textX = static_cast<int32_t>(tickXF - static_cast<float>(_tickLabelFont->getWidth(text)) + 1);
					}

					renderer->renderString(
						Point(textX, tickY + lineLength + _tickLabelOffset),
						_tickLabelFont,
						_tickColor,
						text
					);
				}

				tickValue += tickValueInterval;
				tickXF += tickPixelInterval;
			}
		}

		// Handle
		if (_handleColor) {
			if (_handleSize == bounds.getHeight() && _handleCornerRadius == handleWidthHalf) {
				renderer->renderFilledCircle(
					Point(
						bounds.getX() + handleCenterLocal,
						bounds.getY() + handleWidthHalf
					),
					_handleCornerRadius,
					_handleColor
				);
			}
			else {
				renderer->renderFilledRectangle(
					Bounds(
						bounds.getX() + handleCenterLocal - handleWidthHalf,
						bounds.getY(),
						_handleSize.getWidth(),
						_handleSize.getHeight()
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

		setValue(_minimumValue + static_cast<float>(localX) / bounds.getWidth() * (_maximumValue - _minimumValue));
	}

	std::wstring_view Slider::percentTickLabelBuilder(const float value) {
		return std::to_wstring(static_cast<int32_t>(std::round(value * 100.0f)));
	}

	std::wstring_view Slider::int32TickLabelBuilder(const float value) {
		return std::to_wstring(static_cast<int32_t>(std::round(value)));
	}
}
