#pragma once

#include "stackLayout.h"
#include "../color.h"
#include "hardware/screen/buffers/generic/screenBuffer.h"
#include "cmath"
#include "traits/backgroundColorAware.h"
#include "traits/foregroundColorAware.h"

namespace yoba {
	class SevenSegment : public BackgroundColorAware, public ForegroundColorAware {
		public:
			static const uint32_t dashes = 0xFFFFFFFF;

			Size onMeasure(ScreenBuffer* screenBuffer, const Size &availableSize) override {
				return {
					(uint16_t) (((getDigitWidth() + getSpacing()) * getDigitCount()) - getSpacing()),
					getDigitHeight()
				};
			}

			void onRender(ScreenBuffer* screenBuffer) override {
				auto bounds = getBounds();
				bounds.setX(bounds.getX() + (getDigitWidth() + getSpacing()) * (getDigitCount() - 1));

				auto value = getValue();

				for (uint8_t i = 0; i < getDigitCount(); i++) {
					if (value == dashes) {
						drawDashes(screenBuffer, bounds.getTopLeft());
					}
					else if (value > 0) {
						drawDigit(screenBuffer, bounds.getTopLeft(), value % 10);

						value /= 10;
					}
					else {
						drawDigit(screenBuffer, bounds.getTopLeft(), 0);
					}

					bounds.setX(bounds.getX() - getDigitWidth() - getSpacing());
				}
			}

			// -------------------------------- Getters & setters --------------------------------

			uint8_t getSegmentThickness() const {
				return _segmentThickness;
			}

			void setSegmentThickness(uint8_t value) {
				_segmentThickness = value;

				invalidate();
			}

			uint8_t getSegmentLength() const {
				return _segmentLength;
			}

			void setSegmentLength(uint8_t value) {
				_segmentLength = value;

				invalidate();
			}

			uint32_t getValue() const {
				return _value;
			}

			void setValue(uint32_t value) {
				_value = value;

				invalidateRender();
			}

			uint8_t getDigitCount() const {
				return _digitCount;
			}

			void setDigitCount(uint8_t value) {
				_digitCount = value;

				invalidate();
			}

			uint8_t getSpacing() const {
				return _spacing;
			}

			void setSpacing(uint8_t value) {
				_spacing = value;

				invalidate();
			}

			uint16_t getDigitWidth() const {
				return (uint16_t) (getSegmentThickness() * 2 + getSegmentLength());
			}

			uint16_t getDigitHeight() const {
				return (uint16_t) (getSegmentThickness() * 3 + getSegmentLength() * 2);
			}

		private:
			uint32_t _value = 0;
			uint8_t _digitCount = 1;
			uint8_t _spacing = 3;
			uint8_t _segmentThickness = 3;
			uint8_t _segmentLength = 9;

			void drawSegments(
				ScreenBuffer* screenBuffer,
				const Point& position,
				bool s0,
				bool s1,
				bool s2,
				bool s3,
				bool s4,
				bool s5,
				bool s6
			) const {
				const uint8_t t = getSegmentThickness();
				const uint8_t l = getSegmentLength();

				screenBuffer->renderFilledRectangle(Bounds(position.getX() + t, position.getY(), l, t), s0 ? getForegroundColor() : getBackgroundColor());
				screenBuffer->renderFilledRectangle(Bounds(position.getX() + t + l, position.getY() + t, t, l), s1 ? getForegroundColor() : getBackgroundColor());
				screenBuffer->renderFilledRectangle(Bounds(position.getX() + t + l, position.getY() + t + l + t, t, l), s2 ? getForegroundColor() : getBackgroundColor());
				screenBuffer->renderFilledRectangle(Bounds(position.getX() + t, position.getY() + (t + l) * 2, l, t), s3 ? getForegroundColor() : getBackgroundColor());
				screenBuffer->renderFilledRectangle(Bounds(position.getX(), position.getY() + t + l + t, t, l), s4 ? getForegroundColor() : getBackgroundColor());
				screenBuffer->renderFilledRectangle(Bounds(position.getX(), position.getY() + t, t, l), s5 ? getForegroundColor() : getBackgroundColor());
				screenBuffer->renderFilledRectangle(Bounds(position.getX() + t, position.getY() + t + l, l, t), s6 ? getForegroundColor() : getBackgroundColor());
			}

			void drawDigit(ScreenBuffer* screenBuffer, const Point& position, uint8_t digit) {
				switch (digit) {
					case 0:
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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
						drawSegments(
							screenBuffer,
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

			void drawDashes(ScreenBuffer* screenBuffer, const Point& position) {
				drawSegments(
					screenBuffer,
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
