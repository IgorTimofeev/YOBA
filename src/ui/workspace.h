#pragma once

#include "layout.h"
#include "event.h"

namespace yoba {
	class Screen;
	class Animation;

	class Workspace : public Layout {
		public:
			Workspace();

			void measure(ScreenBuffer* screenBuffer);
			void arrange();

			void onRender(ScreenBuffer* screenBuffer) override;
			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;

			void tick() override;
			void handleEvent(Event &event) override;

			void startAnimation(Animation* animation) override;

			Element *getCapturedElement() const;
			void setCapturedElement(Element *capturedElement);

		private:
			bool _isRendered = false;
			bool _isMeasured = false;
			bool _isArranged = false;
			Element* _capturedElement = nullptr;
			std::vector<Animation*> _animations {};

			void animate();
	};
}