#pragma once

#include "layout.h"
#include "../hardware/screen/buffers/screenBuffer.h"
#include "../hardware/input/InputDevice.h"
#include "../font.h"

namespace yoba {
	class Animation;

	class Application : public Layout {
		public:
			explicit Application(
				ScreenBuffer* screenBuffer
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

			Callback<>& getOnTick();

			ScreenBuffer* getScreenBuffer() const;
			void setScreenBuffer(ScreenBuffer* screenBuffer);

			void addInputDevice(InputDevice* inputDevice);

			virtual void tick();

		private:
			ScreenBuffer* _screenBuffer;

			bool _isRendered = false;
			bool _isMeasured = false;
			bool _isArranged = false;
			Element* _capturedElement = nullptr;
			std::vector<Animation*> _animations {};
			Callback<> _onTick {};
			const Font* _defaultFont = nullptr;

			void animationsTick();
	};
}