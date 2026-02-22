#pragma once

#include <optional>
#include <functional>

#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/valueElement.h>

namespace YOBA {
	class Slider : public Control,public ValueElement<float> {
		public:
			const Color* getTrackColor() const;
			void setTrackColor(const Color* value);

			const Color* getFillColor() const;
			void setFillColor(const Color* value);

			const Color* getHandleColor() const;
			void setHandleColor(const Color* value);

			uint8_t getTrackSize() const;
			void setTrackSize(const uint8_t value);

			uint8_t getTrackCornerRadius() const;
			void setTrackCornerRadius(const uint8_t value);

			const Size& getHandleSize() const;
			void setHandleSize(const Size& value);

			uint8_t getHandleCornerRadius() const;
			void setHandleCornerRadius(const uint8_t value);

			float getMinimumValue() const;
			void setMinimumValue(const float value);

			float getMaximumValue() const;
			void setMaximumValue(const float value);

			void setValue(const float value) override;
			float getValueFactor() const;

			uint8_t getTickCount() const;
			void setTickCount(const uint8_t value);

			const Color* getTickColor() const;
			void setTickColor(const Color* value);

			uint8_t getSmallTickLineLength() const;
			void setSmallTickLineLength(const uint8_t value);

			uint8_t getBigTickLineLength() const;
			void setBigTickLineLength(const uint8_t value);

			uint8_t getTickOffset() const;
			void setTickOffset(const uint8_t value);

			uint8_t getBigTickStep() const;
			void setBigTickStep(const uint8_t value);

			uint8_t getTickLabelOffset() const;
			void setTickLabelOffset(const uint8_t value);

			const Font* getTickLabelFont() const;
			void setTickLabelFont(const Font& value);

			const std::function<std::wstring_view(float)>& getTickLabelBuilder() const;
			void setTickLabelBuilder(const std::function<std::wstring_view(float)>& value);

			static std::wstring_view percentTickLabelBuilder(const float value);
			static std::wstring_view int32TickLabelBuilder(const float value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;
			void onEvent(Event& event) override;

		private:
			// Value
			float _minimumValue = 0.0f;
			float _maximumValue = 1.0f;

			// Track
			uint8_t _trackSize = 4;
			uint8_t _trackCornerRadius = 2;
			const Color* _trackColor = nullptr;

			// Fill
			const Color* _fillColor = nullptr;

			// Handle
			Size _handleSize {8, 8 };
			uint8_t _handleCornerRadius = 4;
			const Color* _handleColor = nullptr;

			// Tick
			uint8_t _tickCount = 10;
			uint8_t _bigTickStep = 5;
			uint8_t _tickOffset = 0;
			uint8_t _smallTickLineLength = 3;
			uint8_t _bigTickLineLength = 5;
			const Color* _tickColor = nullptr;

			uint8_t _tickLabelOffset = 2;
			const Font* _tickLabelFont = nullptr;
			std::function<std::wstring_view(float)> _tickLabelBuilder = percentTickLabelBuilder;

			void updateValueFromEvent(const PointerEvent& event);
	};
}
