#pragma once

#include "element.h"
#include "vector"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../bounds.h"
#include "../size.h"

namespace yoba {
	class Layout : public Element {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent& event) override;

			void setRoot(Application* value) override;

			size_t getChildrenCount();

			int32_t getIndexOfChild(Element* element);

			virtual void removeChildAt(int index);

			virtual void removeChild(Element* child);

			void removeChildren();

			Element* getChildAt(size_t index);

			virtual void addChild(Element* child);

			virtual Element* operator[](size_t index);

			virtual void operator+=(Element* child);

			virtual void operator-=(Element* child);

		protected:
			std::vector<Element*> _children {};

			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;
	};
}