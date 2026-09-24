#pragma once

#include <unordered_map>

#include <YOBA/UI/Layout.hpp>

namespace YOBA {
	class ManualLayout : public Layout {
		public:
			Point getChildPosition(const Element* child);

			void setChildPosition(const Element* child, const Point& value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
			void onChildRemoved(Element* child) override;

		private:
			std::unordered_map<const Element*, Point> _childPositions {};

	};
}