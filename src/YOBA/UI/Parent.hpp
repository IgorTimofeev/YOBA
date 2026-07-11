#pragma once

#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class Parent : public virtual Element {
		public:


		protected:
			virtual void onChildAdded(Element* child);
			virtual void onChildRemoved(Element* child);
	};
}