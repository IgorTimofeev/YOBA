#pragma once

#include "layout.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../hardware/touch/drivers/touchDriver.h"
#include "../font.h"

namespace yoba {
	class Animation;

	class Application : public Layout {
		public:
			Application(
				ScreenBuffer* screenBuffer,
				TouchDriver* touchDriver
			);

			virtual void setup();

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation value) const;

			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;

			void handleEvent(InputEvent &event) override;
			void startAnimation(Animation* animation) override;

			Element *getCapturedElement() const;
			void setCapturedElement(Element *capturedElement);

			const Font *getDefaultFont() const;
			void setDefaultFont(const Font* defaultFont);

			virtual void tick();

		private:
			ScreenBuffer* _screenBuffer;
			TouchDriver* _touchDriver;

			bool _isRendered = false;
			bool _isMeasured = false;
			bool _isArranged = false;
			Element* _capturedElement = nullptr;
			std::vector<Animation*> _animations {};
			const Font* _defaultFont = nullptr;

			void animationsTick();
	};
}