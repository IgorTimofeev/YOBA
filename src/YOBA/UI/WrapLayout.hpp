#pragma once

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/HorizontalAndVerticalSpacingElement.hpp>

namespace YOBA {
	class WrapLayout : public Layout, public HorizontalAndVerticalSpacingElement {
		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
	};
}