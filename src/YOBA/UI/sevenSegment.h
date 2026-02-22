#pragma once

#include <cmath>
#include <limits>

#include <YOBA/main/color.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/stackLayout.h>

namespace YOBA {
	class SevenSegment : public Control {
		public:
			constexpr static uint8_t decimalSeparatorDisabled = 0xFF;
			constexpr static int32_t valueDashes = std::numeric_limits<int32_t>::min();

			uint8_t getSegmentThickness() const;
			void setSegmentThickness(uint8_t value);
			uint8_t getSegmentLength() const;

			void setSegmentLength(uint8_t value);

			int32_t getValue() const;
			void setValue(int32_t value);

			uint8_t getDigitCount() const;
			void setDigitCount(uint8_t value);

			uint8_t getDigitSpacing() const;
			void setDigitSpacing(uint8_t value);

			uint8_t getDecimalSeparatorSpacing() const;
			void setDecimalSeparatorSpacing(uint8_t value);

			uint8_t getDecimalSeparatorIndex() const;
			void setDecimalSeparatorIndex(uint8_t value);

			uint16_t getDigitWidth() const;
			uint16_t getDigitHeight() const;

			const Color& getInactiveColor() const;
			void setInactiveColor(const Color& value);

			const Color& getActiveColor() const;
			void setActiveColor(const Color& value);

			bool isSignVisible() const;
			void setSignVisible(bool signVisible);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;

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

			// -0000-
			// 5----1
			// -6666-
			// 4----2
			// -3333-
			void renderSegments(
				Renderer& renderer,
				const Point& position,
				bool s0,
				bool s1,
				bool s2,
				bool s3,
				bool s4,
				bool s5,
				bool s6
			) const;

			void renderDigit(Renderer& renderer, const Point& position, uint8_t digit) const;
			void renderEmpty(Renderer& renderer, const Point& position) const;

			void render9VerticalSegment(Renderer& renderer, const Point& position, bool state) const;
			void renderDash(Renderer& renderer, const Point& position) const;

			void renderMinus(Renderer& renderer, const Point& position) const;
			void renderPlus(Renderer& renderer, const Point& position) const;
	};
}
