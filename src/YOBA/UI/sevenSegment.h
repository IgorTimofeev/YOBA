#pragma once

#include "stackLayout.h"
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include "cmath"
#include <YOBA/UI/control.h>

namespace YOBA {
	class SevenSegment : public Control {
		public:
			constexpr static uint32_t noValue = 0xFFFFFFFF;
			constexpr static uint8_t decimalSeparatorDisabled = 0xFF;

			Size onMeasure(const Size& availableSize) override {
				return {
					static_cast<uint16_t>(
						_decimalSeparatorIndex == decimalSeparatorDisabled
						? getDigitWidth() * getDigitCount() + getDigitSpacing() * (getDigitCount() - 1)
						: getDigitWidth() * getDigitCount() + getDigitSpacing() * std::max(getDigitCount() - 2, 0) + getDecimalSeparatorSpacing() * 2 + getSegmentThickness()
					),
					getDigitHeight()
				};
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto position = Point(
					bounds.getX() + (getDigitWidth() + getDigitSpacing()) * (getDigitCount() - 1),
					bounds.getY()
				);

				auto value = getValue();

				for (uint8_t i = 0; i < getDigitCount(); i++) {
					if (value == noValue) {
						renderDashes(renderer, position);
					}
					else if (value > 0) {
						renderDigit(renderer, position, value % 10);

						value /= 10;
					}
					else {
						renderDigit(renderer, position, 0);
					}
					
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

			uint8_t getSegmentThickness() const {
				return _segmentThickness;
			}
			
			void setSegmentThickness(const uint8_t value) {
				_segmentThickness = value;

				invalidate();
			}

			uint8_t getSegmentLength() const {
				return _segmentLength;
			}
			
			void setSegmentLength(const uint8_t value) {
				_segmentLength = value;

				invalidate();
			}

			uint32_t getValue() const {
				return _value;
			}
			
			void setValue(const uint32_t value) {
				_value = value;

				invalidateRender();
			}

			uint8_t getDigitCount() const {
				return _digitCount;
			}
			
			void setDigitCount(const uint8_t value) {
				_digitCount = value;

				invalidate();
			}

			uint8_t getDigitSpacing() const {
				return _digitSpacing;
			}
			
			void setDigitSpacing(const uint8_t value) {
				_digitSpacing = value;

				invalidate();
			}
			
			uint8_t getDecimalSeparatorSpacing() const {
				return _decimalSeparatorSpacing;
			}
			
			void setDecimalSeparatorSpacing(const uint8_t value) {
				_decimalSeparatorSpacing = value;
				
				invalidate();
			}
			
			uint8_t getDecimalSeparatorIndex() const {
				return _decimalSeparatorIndex;
			}
			
			void setDecimalSeparatorIndex(uint8_t value) {
				_decimalSeparatorIndex = value;
				
				invalidate();
			}
			
			uint16_t getDigitWidth() const {
				return static_cast<uint16_t>(getSegmentThickness() * 2 + getSegmentLength());
			}
			
			uint16_t getDigitHeight() const {
				return static_cast<uint16_t>(getSegmentThickness() * 3 + getSegmentLength() * 2);
			}

			const Color* getInactiveColor() const {
				return _inactiveColor;
			}
			void setInactiveColor(const Color* value) {
				_inactiveColor = value;
			}

			const Color* getActiveColor() const {
				return _activeColor;
			}
			void setActiveColor(const Color* value) {
				_activeColor = value;
			}

		private:
			uint32_t _value = 0;
			uint8_t _digitCount = 1;
			uint8_t _digitSpacing = 3;
			uint8_t _decimalSeparatorSpacing = 2;
			uint8_t _decimalSeparatorIndex = decimalSeparatorDisabled;
			uint8_t _segmentThickness = 3;
			uint8_t _segmentLength = 9;

			const Color* _inactiveColor = nullptr;
			const Color* _activeColor = nullptr;

			void renderSegments(
				Renderer* renderer,
				const Point& position,
				const bool s0,
				const bool s1,
				const bool s2,
				const bool s3,
				const bool s4,
				const bool s5,
				const bool s6
			) const {
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

			void renderDigit(Renderer* renderer, const Point& position, const uint8_t digit) const {
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

			void renderDashes(Renderer* renderer, const Point& position) const {
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
	};
}
