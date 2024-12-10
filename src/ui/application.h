#pragma once

#include "layout.h"
#include "../hardware/screen/buffers/generic/screenBuffer.h"
#include "../hardware/input/inputDevice.h"
#include "../font.h"
#include "../resources/fonts/unscii16Font.h"

namespace yoba {
	class Animation;

	class Application : public Layout {
		public:
			// Sexy old school font that will be used as fallback value when rendering
			// child elements that contain text, but doesn't have any specific font
			static const Unscii16Font defaultFont;

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

			const Font* getFont() const;
			void setFont(const Font* font);

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

			const Font* _font = &defaultFont;

			void animationsTick();
	};
}