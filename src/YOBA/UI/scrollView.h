#pragma once

#include <YOBA/UI/layout.h>
#include <YOBA/UI/scrollBar.h>

namespace YOBA {
	enum class ScrollMode : uint8_t {
		computed,
		enabled,
		hidden,
		disabled
	};

	class ScrollView : public Layout {
		public:
			ScrollView();

			ScrollMode getHorizontalScrollMode() const;
			void setHorizontalScrollMode(const ScrollMode value);

			ScrollMode getVerticalScrollMode() const;
			void setVerticalScrollMode(const ScrollMode value);

			uint16_t getHorizontalPosition() const;
			void setHorizontalPosition(const uint16_t value);

			uint16_t getVerticalPosition() const;
			void setVerticalPosition(const uint16_t value);

			void setScrollBarOffset(const uint16_t value);
			void setScrollBarSize(const uint16_t value);
			void setScrollBarCornerRadius(const uint16_t value);
			void setScrollBarThumbColor(const Color* value);

			const Bounds& getContentBounds() const;

			void scrollHorizontallyBy(const int32_t delta);
			void scrollVerticallyBy(const int32_t delta);
			void scrollTo(const Element* element);
			void scrollToCenter(const Element* element);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onCaptureChanged() override;
			void onEventBeforeChildren(Event* event) override;
			void onEventAfterChildren(Event* event) override;

		private:
			ScrollBar _horizontalScrollBar {};
			ScrollBar _verticalScrollBar {};

			ScrollMode _horizontalScrollMode = ScrollMode::disabled;
			ScrollMode _verticalScrollMode = ScrollMode::computed;

			Size _contentMeasuredSize {};
			Bounds _contentBounds {};
			const Element* _scrollIntoViewLaterTo = nullptr;

			bool _horizontalScrollPossible = false;
			bool _verticalScrollPossible = false;

			Point _lastTouchPosition { -1, -1 };
	};
}