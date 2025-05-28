#pragma once

#include <vector>
#include <YOBA/UI/element.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/main/bounds.h>
#include <YOBA/main/size.h>
#include <YOBA/main/event.h>
#include <iterator>

namespace YOBA {
	class Layout : public virtual Element {
		public:
			size_t getChildrenCount() const;
			size_t getIndexOfChild(Element* element);
			Element* getChildAt(size_t index) const;

			void addChild(Element* child);
			void insertChild(size_t index, Element* child);
			void insertChildFromEnd(size_t offset, Element* child);

			void removeChildAt(int index);
			void removeChild(Element* child);
			void removeChildren();
			void moveChildrenTo(Layout* layout);
			void removeAndDeleteChildren();

			virtual void pushEvent(Event* event);

			std::vector<Element*>::iterator begin();
			std::vector<Element*>::iterator end();

			Element* operator[](size_t index) const;
			Layout& operator+=(Element* child);
			Layout& operator-=(Element* child);

		protected:
			void handleEvent(Event* event, bool callHandlers) override;
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			virtual void onEventBeforeChildren(Event* event);
			virtual void onEventAfterChildren(Event* event);

			virtual void onChildAdded(Element* child);
			virtual void onChildRemoved(Element* child);

		private:
			std::vector<Element*> _children {};
	};
}