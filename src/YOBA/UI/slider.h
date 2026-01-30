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

			float getValueMinimum() const;
			void setValueMinimum(const float value);

			float getValueMaximum() const;
			void setValueMaximum(const float value);

			void setValue(const float value) override;
			float getValueFactor() const;

			float getTickInterval() const;
			void setTickInterval(const float value);

			const Color* getTickColor() const;
			void setTickColor(const Color* value);

			uint8_t getSmallTickLength() const;
			void setSmallTickLength(const uint8_t value);

			uint8_t getBigTickLength() const;
			void setBigTickLength(const uint8_t value);

			uint8_t getTickOffset() const;
			void setTickOffset(const uint8_t value);

			uint8_t getBigTickIndex() const;
			void setBigTickIndex(const uint8_t value);

			uint8_t getTickLabelOffset() const;
			void setTickLabelOffset(const uint8_t value);

			const Font* getTickLabelFont() const;
			void setTickLabelFont(const Font* value);

			const std::function<std::wstring_view(float)>& getTickLabelBuilder() const;
			void setTickLabelBuilder(const std::function<std::wstring_view(float)>& value);

			static std::wstring_view percentTickLabelBuilder(const float value);
			static std::wstring_view int32TickLabelBuilder(const float value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			// Value
			float _valueMinimum = 0.0f;
			float _valueMaximum = 1.0f;

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
			float _tickInterval = 1.0f;
			uint8_t _bigTickIndex = 1;
			uint8_t _tickOffset = 1;
			uint8_t _smallTickLength = 2;
			uint8_t _bigTickLength = 4;
			const Color* _tickColor = nullptr;

			uint8_t _tickLabelOffset = 1;
			const Font* _tickLabelFont = nullptr;
			std::function<std::wstring_view(float)> _tickLabelBuilder = percentTickLabelBuilder;

			void updateValueFromEvent(const PointerEvent* event);
	};
}
