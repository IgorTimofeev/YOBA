#pragma once

#include "layout.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../hardware/touch/drivers/touchDriver.h"
#include "../resources/fonts/font.h"

namespace yoba {
	class Animation;

	class Application : public Layout {
		public:
			Application(
				ScreenBuffer* screenBuffer,
				TouchDriver* touchDriver
			);

			virtual void begin();

			ScreenOrientation getOrientation() const {
				return _screenBuffer->getDriver()->getOrientation();
			}

			void setOrientation(ScreenOrientation value) const {
				return _screenBuffer->getDriver()->setOrientation(value);
			}

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

			const Font *getDefaultFont() const;
			void setDefaultFont(const Font* defaultFont);

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