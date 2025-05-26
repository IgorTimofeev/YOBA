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
			void removeAndDeleteChildren();

			std::vector<Element*>::iterator begin();
			std::vector<Element*>::iterator end();

			Element* operator[](size_t index) const;
			Layout& operator+=(Element* child);
			Layout& operator-=(Element* child);

		protected:
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;

			void pushEvent(Event* event) override;

			virtual void onEventBeforeChildren(Event* event);
			virtual void onEventAfterChildren(Event* event);

			virtual void onTouchDownBeforeChildren(TouchDownEvent* event);
			virtual void onTouchDownAfterChildren(TouchDownEvent* event);

			virtual void onTouchDragBeforeChildren(TouchDragEvent* event);
			virtual void onTouchDragAfterChildren(TouchDragEvent* event);

			virtual void onTouchUpBeforeChildren(TouchUpEvent* event);
			virtual void onTouchUpAfterChildren(TouchUpEvent* event);

			virtual void onPinchDownBeforeChildren(PinchDownEvent* event);
			virtual void onPinchDownAfterChildren(PinchDownEvent* event);

			virtual void onPinchDragBeforeChildren(PinchDragEvent* event);
			virtual void onPinchDragAfterChildren(PinchDragEvent* event);

			virtual void onPinchUpBeforeChildren(PinchUpEvent* event);
			virtual void onPinchUpAfterChildren(PinchUpEvent* event);

			virtual void onChildAdded(Element* child);
			virtual void onChildRemoved(Element* child);

		private:
			std::vector<Element*> _children {};
	};
}