#pragma once

#include "element.h"
#include "vector"
#include "hardware/screen/buffers/screenBuffer.h"
#include "../bounds.h"
#include "../size.h"
#include "../event.h"
#include <iterator>

namespace yoba {
	class Container : public virtual Element {
		public:
			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent& event) override;

			size_t getChildrenCount();

			int32_t getIndexOfChild(Element* element);

			virtual void removeChildAt(int index);

			virtual void removeChild(Element* child);

			void removeChildren();

			Element* getChildAt(size_t index);

			virtual void addChild(Element* child);

			std::vector<Element*>::iterator begin();

			std::vector<Element*>::iterator end();

			virtual Element* operator[](size_t index);

			virtual void operator+=(Element* child);

			virtual void operator-=(Element* child);

		protected:
			void setApplication(Application* value) override;

			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;

		private:
			std::vector<Element*> _children {};
	};
}