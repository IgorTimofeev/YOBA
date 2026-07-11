#pragma once

#include <YOBA/UI/Decorator.hpp>
#include <YOBA/UI/ScrollBar.hpp>
#include <YOBA/UI/Transforms/MarginTransform.hpp>

namespace YOBA {
	enum class ScrollMode : uint8_t {
		computed,
		enabled,
		hidden,
		disabled
	};

	class ScrollView : public Decorator {
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

			const Rectangle& getContentBounds() const;

			void scrollHorizontallyBy(const int32_t delta);
			void scrollVerticallyBy(const int32_t delta);
			void scrollTo(const Element* element);
			void scrollToCenter(const Element* element);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;
			void onCaptureChanged() override;
			void onEventBeforeChild(Event* event) override;
			void onEventAfterChild(Event* event) override;

		private:
			MarginTransform _horizontalTransform {};
			ScrollBar _horizontalScrollBar {};

			MarginTransform _verticalTransform {};
			ScrollBar _verticalScrollBar {};

			ScrollMode _horizontalScrollMode = ScrollMode::disabled;
			ScrollMode _verticalScrollMode = ScrollMode::computed;

			Size _contentMeasuredSize {};
			Rectangle _contentBounds {};
			const Element* _scrollIntoViewLaterTo = nullptr;

			Point _lastTouchPosition { -1, -1 };

			void onArrangePass(const Rectangle& bounds);
	};
}