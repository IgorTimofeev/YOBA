#pragma once

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/HorizontalAndVerticalGapElement.hpp>

namespace YOBA {
	class WrapLayout : public Layout, public HorizontalAndVerticalGapElement {
		public:
			WrapLayout() = default;
			WrapLayout(const uint16_t horizontalGap, const uint16_t verticalGap);
			WrapLayout(const uint16_t gap);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
	};
}