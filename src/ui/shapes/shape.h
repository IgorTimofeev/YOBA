#pragma once

#include "rendering/renderers/renderer.h"
#include "ui/traits/fillColorElement.h"

namespace yoba::ui {
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
