#pragma once

#include "element.h"
#include "vector"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../bounds.h"
#include "../size.h"

namespace yoba {
	class Layout : public Element {
		public:
			void tick() override;

			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(Event& event) override;

			size_t getChildrenCount();

			int32_t getIndexOfChild(Element* element);

			void removeChildAt(int index);

			void removeChild(Element* child);

			void removeChildren();

			Element* getChildAt(size_t index);

			void addChild(Element* child);

			virtual Element* operator[](size_t index);

			virtual void operator+=(Element* child);

			virtual void operator-=(Element* child);

		protected:
			std::vector<Element*> _children {};

			Size getDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;
	};
}