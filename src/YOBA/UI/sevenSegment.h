#pragma once

#include "stackLayout.h"
#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include "cmath"
#include <YOBA/UI/control.h>

namespace YOBA {
	class SevenSegment : public Control {
		public:
			constexpr static uint8_t decimalSeparatorDisabled = 0xFF;

			uint8_t getSegmentThickness() const;
			void setSegmentThickness(const uint8_t value);
			uint8_t getSegmentLength() const;

			void setSegmentLength(const uint8_t value);

			int32_t getValue() const;
			void setValue(const int32_t value);

			uint8_t getDigitCount() const;
			void setDigitCount(const uint8_t value);

			uint8_t getDigitSpacing() const;
			void setDigitSpacing(const uint8_t value);

			uint8_t getDecimalSeparatorSpacing() const;
			void setDecimalSeparatorSpacing(const uint8_t value);

			uint8_t getDecimalSeparatorIndex() const;
			void setDecimalSeparatorIndex(const uint8_t value);

			uint16_t getDigitWidth() const;
			uint16_t getDigitHeight() const;

			const Color* getInactiveColor() const;
			void setInactiveColor(const Color* value);

			const Color* getActiveColor() const;
			void setActiveColor(const Color* value);

			bool isSignVisible() const;
			void setSignVisible(const bool signVisible);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			int32_t _value = 0;
			uint8_t _digitCount = 1;
			uint8_t _digitSpacing = 3;
			uint8_t _decimalSeparatorSpacing = 2;
			uint8_t _decimalSeparatorIndex = decimalSeparatorDisabled;
			uint8_t _segmentThickness = 3;
			uint8_t _segmentLength = 9;
			bool _signVisible = false;

			const Color* _inactiveColor = nullptr;
			const Color* _activeColor = nullptr;

			// -1111-
			// 6----2
			// -7777-
			// 5----3
			// -4444-
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
			) const;

			void renderDigit(Renderer* renderer, const Point& position, const uint8_t digit) const;
			void renderEmpty(Renderer* renderer, const Point& position) const;
			void renderMinus(Renderer* renderer, const Point& position) const;
			void renderPlus(Renderer* renderer, const Point& position) const;
	};
}
