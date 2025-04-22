#pragma once

#include "YOBA/main/rendering/renderer.h"
#include "YOBA/UI/traits/fillColorElement.h"

namespace YOBA {
	class Shape : public FillColorElement {
		protected:
			void onEvent(Event* event) override {
				Element::onEvent(event);

				if (ScreenEvent::isScreen(event)) {
					event->setHandled(true);
				}
			}
	};
}
