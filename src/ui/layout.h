#pragma once

#include "element.h"
#include "vector"
#include "rendering/renderers/renderer.h"
#include "main/bounds.h"
#include "main/size.h"
#include "main/event.h"
#include <iterator>

namespace yoba::ui {
	class Layout : public virtual Element {
		public:
			size_t getChildrenCount();
			int32_t getIndexOfChild(Element* element);
			Element* getChildAt(size_t index);

			void addChild(Element* child);
			void insertChild(size_t index, Element* element);
			void insertChildFromEnd(size_t offset, Element* child);

			virtual void removeChildAt(int index);
			virtual void removeChild(Element* child);
			void removeChildren();

			std::vector<Element*>::iterator begin();
			std::vector<Element*>::iterator end();

			virtual Element* operator[](size_t index);
			virtual void operator+=(Element* child);
			virtual void operator-=(Element* child);

		protected:
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;

			virtual void onChildAdded(Element* child);
			virtual void onChildRemoved(Element* child);

			void setApplication(Application* value) override;

		private:
			std::vector<Element*> _children;
	};
}