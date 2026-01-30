#pragma once

#include <unordered_map>

#include <YOBA/UI/stackLayout.h>

namespace YOBA {
	class RelativeStackLayout : public StackLayout {
		public:
			RelativeStackLayout();
			RelativeStackLayout(const Orientation orientation);
			RelativeStackLayout(const uint16_t spacing);
			RelativeStackLayout(const Orientation orientation, const uint16_t spacing);

			constexpr static float autoSize = -1;

			float getRelativeSize(const Element* child);
			void setRelativeSize(const Element* child, float value);

			bool isAutoSize(const Element* child);
			void setAutoSize(const Element* child, const bool value = true);

		protected:
			Size onMeasure(const Size& availableSize) override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void onChildRemoved(Element* child) override {
				Layout::onChildRemoved(child);

				tryRemoveRelativeSize(child);
			}

		private:
			std::unordered_map<const Element*, float> _elementSizes {};

			void tryRemoveRelativeSize(const Element* child);
	};
}