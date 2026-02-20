#include <YOBA/UI/sevenSegment.h>

namespace YOBA {
	uint8_t SevenSegment::getSegmentThickness() const {
		return _segmentThickness;
	}

	void SevenSegment::setSegmentThickness(const uint8_t value) {
		_segmentThickness = value;

		invalidate();
	}

	uint8_t SevenSegment::getSegmentLength() const {
		return _segmentLength;
	}

	void SevenSegment::setSegmentLength(const uint8_t value) {
		_segmentLength = value;

		invalidate();
	}

	int32_t SevenSegment::getValue() const {
		return _value;
	}

	void SevenSegment::setValue(const int32_t value) {
		_value = value;

		invalidateRender();
	}

	uint8_t SevenSegment::getDigitCount() const {
		return _digitCount;
	}

	void SevenSegment::setDigitCount(const uint8_t value) {
		_digitCount = value;

		invalidate();
	}

	uint8_t SevenSegment::getDigitSpacing() const {
		return _digitSpacing;
	}

	void SevenSegment::setDigitSpacing(const uint8_t value) {
		_digitSpacing = value;

		invalidate();
	}

	uint8_t SevenSegment::getDecimalSeparatorSpacing() const {
		return _decimalSeparatorSpacing;
	}

	void SevenSegment::setDecimalSeparatorSpacing(const uint8_t value) {
		_decimalSeparatorSpacing = value;

		invalidate();
	}

	uint8_t SevenSegment::getDecimalSeparatorIndex() const {
		return _decimalSeparatorIndex;
	}

	void SevenSegment::setDecimalSeparatorIndex(const uint8_t value) {
		_decimalSeparatorIndex = value;

		invalidate();
	}

	uint16_t SevenSegment::getDigitWidth() const {
		return static_cast<uint16_t>(getSegmentThickness() * 2 + getSegmentLength());
	}

	uint16_t SevenSegment::getDigitHeight() const {
		return static_cast<uint16_t>(getSegmentThickness() * 3 + getSegmentLength() * 2);
	}

	const Color* SevenSegment::getInactiveColor() const {
		return _inactiveColor;
	}

	void SevenSegment::setInactiveColor(const Color* value) {
		_inactiveColor = value;
	}

	const Color* SevenSegment::getActiveColor() const {
		return _activeColor;
	}

	void SevenSegment::setActiveColor(const Color* value) {
		_activeColor = value;
	}

	bool SevenSegment::isSignVisible() const {
		return _signVisible;
	}

	void SevenSegment::setSignVisible(const bool signVisible) {
		_signVisible = signVisible;
	}

	Size SevenSegment::onMeasure(const Size& availableSize) {
		return {
			static_cast<uint16_t>(
				_decimalSeparatorIndex == decimalSeparatorDisabled
					? getDigitWidth() * getDigitCount() + getDigitSpacing() * (getDigitCount() - 1)
					: getDigitWidth() * getDigitCount() + getDigitSpacing() * std::max(getDigitCount() - 2, 0) + getDecimalSeparatorSpacing() * 2 + getSegmentThickness()
			),
			getDigitHeight()
		};
	}

	void SevenSegment::onRender(Renderer* renderer, const Bounds& bounds) {
		auto position = Point(
			bounds.getX() + (getDigitWidth() + getDigitSpacing()) * (getDigitCount() - 1),
			bounds.getY()
		);

		const auto value = getValue();
		auto valueAbs = std::abs(value);

		for (uint8_t i = 0; i < getDigitCount(); i++) {
			// Last digit
			if (i == getDigitCount() - 1) {
				if (_signVisible) {
					if (value >= 0) {
						renderEmpty(renderer, position);
					}
					else {
						renderMinus(renderer, position);
					}
				}
				else {
					renderDigit(renderer, position, valueAbs % 10);
				}
			}
			// Non-last
			else {
				if (valueAbs > 0) {
					renderDigit(renderer, position, valueAbs % 10);

					valueAbs /= 10;
				}
				else {
					renderDigit(renderer, position, 0);
				}
			}

			// Decimal separator
			if (i == _decimalSeparatorIndex) {
				renderer->renderFilledRectangle(
					Bounds(position.getX() - _decimalSeparatorSpacing - 1, bounds.getY2() - _segmentThickness + 1, _segmentThickness, _segmentThickness),
					_activeColor
				);

				position.setX(position.getX() - getDigitWidth() - _decimalSeparatorSpacing * 2 - _segmentThickness);
			}
			else {
				position.setX(position.getX() - getDigitWidth() - getDigitSpacing());
			}
		}
	}

	void SevenSegment::renderSegments(Renderer* renderer, const Point& position, const bool s0, const bool s1, const bool s2, const bool s3, const bool s4, const bool s5, const bool s6) const {
		const uint8_t t = getSegmentThickness();
		const uint8_t l = getSegmentLength();

		renderer->renderFilledRectangle(Bounds(position.getX() + t, position.getY(), l, t), s0 ? _activeColor : _inactiveColor);
		renderer->renderFilledRectangle(Bounds(position.getX() + t + l, position.getY() + t, t, l), s1 ? _activeColor : _inactiveColor);
		renderer->renderFilledRectangle(Bounds(position.getX() + t + l, position.getY() + t + l + t, t, l), s2 ? _activeColor : _inactiveColor);
		renderer->renderFilledRectangle(Bounds(position.getX() + t, position.getY() + (t + l) * 2, l, t), s3 ? _activeColor : _inactiveColor);
		renderer->renderFilledRectangle(Bounds(position.getX(), position.getY() + t + l + t, t, l), s4 ? _activeColor : _inactiveColor);
		renderer->renderFilledRectangle(Bounds(position.getX(), position.getY() + t, t, l), s5 ? _activeColor : _inactiveColor);
		renderer->renderFilledRectangle(Bounds(position.getX() + t, position.getY() + t + l, l, t), s6 ? _activeColor : _inactiveColor);
	}

	void SevenSegment::renderDigit(Renderer* renderer, const Point& position, const uint8_t digit) const {
		switch (digit) {
			case 0:
				renderSegments(
					renderer,
					position,
					true,
					true,
					true,
					true,
					true,
					true,
					false
				);

				break;

			case 1:
				renderSegments(
					renderer,
					position,
					false,
					true,
					true,
					false,
					false,
					false,
					false
				);

				break;

			case 2:
				renderSegments(
					renderer,
					position,
					true,
					true,
					false,
					true,
					true,
					false,
					true
				);

				break;

			case 3:
				renderSegments(
					renderer,
					position,
					true,
					true,
					true,
					true,
					false,
					false,
					true
				);

				break;

			case 4:
				renderSegments(
					renderer,
					position,
					false,
					true,
					true,
					false,
					false,
					true,
					true
				);

				break;

			case 5:
				renderSegments(
					renderer,
					position,
					true,
					false,
					true,
					true,
					false,
					true,
					true
				);

				break;

			case 6:
				renderSegments(
					renderer,
					position,
					true,
					false,
					true,
					true,
					true,
					true,
					true
				);

				break;

			case 7:
				renderSegments(
					renderer,
					position,
					true,
					true,
					true,
					false,
					false,
					false,
					false
				);

				break;

			case 8:
				renderSegments(
					renderer,
					position,
					true,
					true,
					true,
					true,
					true,
					true,
					true
				);

				break;

			default:
				renderSegments(
					renderer,
					position,
					true,
					true,
					true,
					true,
					false,
					true,
					true
				);

				break;
		}
	}

	void SevenSegment::renderEmpty(Renderer* renderer, const Point& position) const {
		renderSegments(
			renderer,
			position,
			false,
			false,
			false,
			false,
			false,
			false,
			false
		);
	}

	void SevenSegment::renderMinus(Renderer* renderer, const Point& position) const {
		renderSegments(
			renderer,
			position,
			false,
			false,
			false,
			false,
			false,
			false,
			true
		);
	}

	void SevenSegment::renderPlus(Renderer* renderer, const Point& position) const {
		renderSegments(
			renderer,
			position,
			false,
			true,
			true,
			false,
			false,
			false,
			true
		);
	}
}
