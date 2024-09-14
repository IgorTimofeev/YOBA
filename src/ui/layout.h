#pragma once

#ifndef YOBA_LAYOUT_H
#define YOBA_LAYOUT_H
#endif

#include "element.h"
#include "bounds.h"
#include "vector"

namespace yoba {
	class Layout : public Element {
		public:
			void tick() override;

			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(Event& event) override;

			std::vector<Element*>::iterator begin();

			std::vector<Element*>::iterator end();

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
			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;

		private:
			std::vector<Element*> _children {};
	};
}