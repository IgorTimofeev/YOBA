#pragma once

#include <limits>

#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/valueElement.h>

namespace YOBA {
	class Slider : public Control, public ValueElement<uint16_t> {
		public:
			constexpr static uint16_t valueMax = std::numeric_limits<uint16_t>::max();

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

			uint8_t getHandleSize() const;
			void setHandleSize(const uint8_t value);

			uint8_t getHandleCornerRadius() const;
			void setHandleCornerRadius(const uint8_t value);

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

		private:
			uint8_t _trackSize = 4;
			uint8_t _trackCornerRadius = 2;

			uint8_t _handleSize = 8;
			uint8_t _handleCornerRadius = 4;

			const Color* _trackColor = nullptr;
			const Color* _fillColor = nullptr;
			const Color* _handleColor = nullptr;

			void updateValueFromEvent(const PointerEvent* event);
	};
}
