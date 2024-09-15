#pragma once

#include "stackLayout.h"
#include "color.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "cmath"
#include "ui/traits/backgroundAware.h"
#include "ui/traits/foregroundAware.h"

namespace yoba {
	class SevenSegment : public BackgroundAware, public ForegroundAware {
		public:
			SevenSegment() {
				setBackground(&Color16::black);
				setForeground(&Color16::white);
			}

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
					if (value > 0) {
						drawDigit(screenBuffer, bounds.getPosition(), value % 10);

						value /= 10;
					}
					else {
						drawDigit(screenBuffer, bounds.getPosition(), 0);
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
				uint8_t t = getSegmentThickness();
				uint8_t l = getSegmentLength();

				screenBuffer->renderRectangle(Bounds(position.getX() + t, position.getY(), l, t), s0 ? getForeground() : getBackground());
				screenBuffer->renderRectangle(Bounds(position.getX() + t + l, position.getY() + t, t, l), s1 ? getForeground() : getBackground());
				screenBuffer->renderRectangle(Bounds(position.getX() + t + l, position.getY() + t + l + t, t, l), s2 ? getForeground() : getBackground());
				screenBuffer->renderRectangle(Bounds(position.getX() + t, position.getY() + (t + l) * 2, l, t), s3 ? getForeground() : getBackground());
				screenBuffer->renderRectangle(Bounds(position.getX(), position.getY() + t + l + t, t, l), s4 ? getForeground() : getBackground());
				screenBuffer->renderRectangle(Bounds(position.getX(), position.getY() + t, t, l), s5 ? getForeground() : getBackground());
				screenBuffer->renderRectangle(Bounds(position.getX() + t, position.getY() + t + l, l, t), s6 ? getForeground() : getBackground());
			}

			void drawDigit(ScreenBuffer* screenBuffer, const Point& position, uint8_t digit) {
				switch (digit) {
					case 0:
						drawSegments(
							display,
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
							display,
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
							display,
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
							display,
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
							display,
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
							display,
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
							display,
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
							display,
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
							display,
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
							display,
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
	};
}
