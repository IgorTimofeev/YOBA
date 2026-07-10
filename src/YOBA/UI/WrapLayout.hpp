#pragma once

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/HorizontalAndVerticalGapElement.hpp>

namespace YOBA {
	class WrapLayout : public Layout, public HorizontalAndVerticalGapElement {
		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
	};
}