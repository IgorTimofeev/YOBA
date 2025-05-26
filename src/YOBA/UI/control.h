#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/main/event.h>

namespace YOBA {
	class Control : public virtual Element {
		public:
			void pushEvent(Event* event) override;

		protected:
			virtual void onEvent(Event* event);

			virtual void onTouchDown(TouchDownEvent* event);
			virtual void onTouchDrag(TouchDragEvent* event);
			virtual void onTouchUp(TouchUpEvent* event);

			virtual void onPinchDown(PinchDownEvent* event);
			virtual void onPinchDrag(PinchDragEvent* event);
			virtual void onPinchUp(PinchUpEvent* event);

		private:

	};
}