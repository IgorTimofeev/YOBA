#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/Core/Events/Event.hpp>

namespace YOBA {
	class Control : public virtual Element {
		public:
			void handleEvent(Event* event, const Rectangle& parentBounds, bool callHandlers) override;

		protected:
			// Control will mark ScreenEvents as handled by default
			virtual void onEvent(Event* event);
	};
}