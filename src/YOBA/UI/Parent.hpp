#pragma once

#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class Parent : public virtual Element {
		public:
			virtual Parent& operator+=(Element* child) = 0;
			virtual Parent& operator-=(Element* child) = 0;

		protected:
			virtual void onChildAdded(Element* child);
			virtual void onChildRemoved(Element* child);

			void handleEvent(Event* event, const Rectangle& parentBounds, bool callHandlers) override;

			virtual void onParentHandleEventBeforeChildren(Event* event) = 0;
			virtual void onParentHandlePointerEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) = 0;
			virtual void onParentHandleEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) = 0;
			virtual void onParentHandleEventAfterChildren(Event* event) = 0;
	};
}