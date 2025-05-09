#pragma once

#include "YOBA/UI/element.h"
#include "vector"
#include "YOBA/main/rendering/renderer.h"
#include "YOBA/main/bounds.h"
#include "YOBA/main/size.h"
#include "YOBA/main/event.h"
#include <iterator>

namespace YOBA {
	class Layout : public virtual Element {
		public:
			size_t getChildrenCount() const;
			int32_t getIndexOfChild(Element* element);
			Element* getChildAt(size_t index) const;

			void addChild(Element* child);
			void insertChild(size_t index, Element* child);
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
			void onRender(Renderer* renderer) override;
			void onEvent(Event* event) override;

			virtual void onChildAdded(Element* child);
			virtual void onChildRemoved(Element* child);

			void setApplication(Application* value) override;

		private:
			std::vector<Element*> _children {};
	};
}