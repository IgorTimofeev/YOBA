#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/main/events/event.h>

namespace YOBA {
	class Control : public virtual Element {
		public:
			void handleEvent(Event* event, bool callHandlers) override;

		protected:
			// Control will mark ScreenEvents as handled by default
			virtual void onEvent(Event* event);

		private:

	};
}