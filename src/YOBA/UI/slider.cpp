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

	float Slider::getValueMinimum() const {
		return _valueMinimum;
	}

	void Slider::setValueMinimum(const float value) {
		_valueMinimum = value;
	}

	float Slider::getValueMaximum() const {
		return _valueMaximum;
	}

	void Slider::setValueMaximum(const float value) {
		_valueMaximum = value;
	}

	void Slider::setValue(const float value) {
		ValueElement::setValue(std::clamp(value, _valueMinimum, _valueMaximum));
	}

	float Slider::getValueFactor() const {
		return (getValue() - _valueMinimum) / (_valueMaximum - _valueMinimum);
	}

	float Slider::getTickInterval() const {
		return _tickInterval;
	}

	void Slider::setTickInterval(const float value) {
		_tickInterval = value;
	}

	const Color* Slider::getTickColor() const {
		return _tickColor;
	}

	void Slider::setTickColor(const Color* value) {
		_tickColor = value;
	}

	uint8_t Slider::getSmallTickLength() const {
		return _smallTickLength;
	}

	void Slider::setSmallTickLength(const uint8_t value) {
		_smallTickLength = value;
	}

	uint8_t Slider::getBigTickLength() const {
		return _bigTickLength;
	}

	void Slider::setBigTickLength(const uint8_t value) {
		_bigTickLength = value;
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

	uint8_t Slider::getBigTickIndex() const {
		return _bigTickIndex;
	}

	void Slider::setBigTickIndex(const uint8_t value) {
		_bigTickIndex = value;
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
		auto height = std::max<uint16_t>(_handleSize.getHeight(), _trackSize);

		if (_tickInterval > 0) {
			height += _tickOffset + _bigTickLength;

			if (_tickLabelFont) {
				height += _tickLabelOffset + _tickLabelFont->getHeight();
			}
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
		const auto widthWithoutHandle = bounds.getWidth() - _handleSize.getWidth();

		const auto handleCenterLocal =
			handleWidthHalf
			+ static_cast<uint16_t>(valueFactor * static_cast<float>(widthWithoutHandle));

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
		if (_tickInterval > 0 && _tickColor) {
			const auto tickY = trackY + _trackSize + _tickOffset;

			float tickXF = bounds.getX() + handleWidthHalf;
			float tickValue = _valueMinimum;
			const auto tickPixelInterval = _tickInterval / (_valueMaximum - _valueMinimum) * widthWithoutHandle;
			uint16_t tickIndex = 0;
			const auto tickToX = bounds.getX2();

			while (tickValue <= _valueMaximum) {
				const auto isBig = tickIndex % _bigTickIndex == 0;
				const auto lineLength = isBig ? _bigTickLength : _smallTickLength;

				// Line
				renderer->renderVerticalLine(
					Point(static_cast<int32_t>(tickXF), tickY),
					lineLength,
					_tickColor
				);

				// Text
				if (isBig && _tickLabelFont) {
					const auto text = std::wstring(_tickLabelBuilder(tickValue));

					renderer->renderString(
						Point(
							static_cast<int32_t>(tickXF - _tickLabelFont->getWidth(text) / 2),
							tickY + lineLength + _tickLabelOffset
						),
						_tickLabelFont,
						_tickColor,
						text
					);
				}

				tickValue += _tickInterval;
				tickXF += tickPixelInterval;
				tickIndex++;
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

		setValue(_valueMinimum + static_cast<float>(localX) / bounds.getWidth() * (_valueMaximum - _valueMinimum));
	}

	std::wstring_view Slider::percentTickLabelBuilder(const float value) {
		return std::to_wstring(static_cast<int32_t>(std::round(value * 100.0f)));
	}

	std::wstring_view Slider::int32TickLabelBuilder(const float value) {
		return std::to_wstring(static_cast<int32_t>(std::round(value)));
	}
}
