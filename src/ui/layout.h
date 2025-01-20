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
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Bounds& bounds) override;
			void onRender(Renderer* renderer) override;
			void onEvent(Event& event) override;

			virtual void onChildAdded(Element* element);
			virtual void onChildRemoved(Element* element);

			void setApplication(Application* value) override;

		private:
			std::vector<Element*> _children;
	};
}