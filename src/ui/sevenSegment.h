#pragma once

#include "stackLayout.h"
#include "main/color.h"
#include "rendering/renderers/renderer.h"
#include "cmath"
#include "ui/traits/primaryColorElement.h"
#include "ui/traits/secondaryColorElement.h"

namespace yoba::ui {
	class SevenSegment : public PrimaryColorElement, public SecondaryColorElement {
		public:
			constexpr static const uint32_t dashes = 0xFFFFFFFF;

			Size onMeasure(const Size& availableSize) override {
				return {
					(uint16_t) (((getDigitWidth() + getSpacing()) * getDigitCount()) - getSpacing()),
					getDigitHeight()
				};
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				auto position = Point(
					bounds.getX() + (getDigitWidth() + getSpacing()) * (getDigitCount() - 1),
					bounds.getY()
				);

				auto value = getValue();

				for (uint8_t i = 0; i < getDigitCount(); i++) {
					if (value == dashes) {
						renderDashes(renderer, position);
					}
					else if (value > 0) {
						renderDigit(renderer, position, value % 10);

						value /= 10;
					}
					else {
						renderDigit(renderer, position, 0);
					}

					position.setX(position.getX() - getDigitWidth() - getSpacing());
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

			void renderSegments(
				Renderer* renderer,
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

				renderer->renderFilledRectangle(Bounds(position.getX() + t, position.getY(), l, t), s0 ? getSecondaryColor() : getPrimaryColor());
				renderer->renderFilledRectangle(Bounds(position.getX() + t + l, position.getY() + t, t, l), s1 ? getSecondaryColor() : getPrimaryColor());
				renderer->renderFilledRectangle(Bounds(position.getX() + t + l, position.getY() + t + l + t, t, l), s2 ? getSecondaryColor() : getPrimaryColor());
				renderer->renderFilledRectangle(Bounds(position.getX() + t, position.getY() + (t + l) * 2, l, t), s3 ? getSecondaryColor() : getPrimaryColor());
				renderer->renderFilledRectangle(Bounds(position.getX(), position.getY() + t + l + t, t, l), s4 ? getSecondaryColor() : getPrimaryColor());
				renderer->renderFilledRectangle(Bounds(position.getX(), position.getY() + t, t, l), s5 ? getSecondaryColor() : getPrimaryColor());
				renderer->renderFilledRectangle(Bounds(position.getX() + t, position.getY() + t + l, l, t), s6 ? getSecondaryColor() : getPrimaryColor());
			}

			void renderDigit(Renderer* renderer, const Point& position, uint8_t digit) {
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

			void renderDashes(Renderer* renderer, const Point& position) {
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
