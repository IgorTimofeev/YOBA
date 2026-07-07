#pragma once

#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/OrientationElement.hpp>
#include <YOBA/UI/Traits/GapElement.hpp>

namespace YOBA {
	class StackLayout : public Layout, public OrientationElement, public GapElement {
		public:
			StackLayout() = default;
			StackLayout(const Orientation orientation);
			StackLayout(const uint16_t gap);
			StackLayout(const Orientation orientation, const uint16_t gap);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
	};
}