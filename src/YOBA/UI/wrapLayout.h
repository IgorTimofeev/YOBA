#pragma once

#include <YOBA/UI/layout.h>
#include <YOBA/UI/traits/horizontalAndVerticalSpacingElement.h>

namespace YOBA {
	class WrapLayout : public Layout, public HorizontalAndVerticalSpacingElement {
		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;
	};
}