#pragma once

#include <YOBA/UI/layout.h>
#include <YOBA/UI/traits/orientationElement.h>
#include <YOBA/UI/traits/gapElement.h>

namespace YOBA {
	class StackLayout : public Layout, public OrientationElement, public GapElement {
		public:
			StackLayout() = default;
			StackLayout(const Orientation orientation);
			StackLayout(const uint16_t gap);
			StackLayout(const Orientation orientation, const uint16_t gap);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Bounds& bounds) override;
	};
}