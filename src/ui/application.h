#pragma once

#include "container.h"
#include "hardware/screen/buffers/screenBuffer.h"
#include "../hardware/input/inputDevice.h"
#include "../font.h"
#include "../resources/fonts/unscii16Font.h"

namespace yoba {
	class Animation;

	class Application : public Container {
		public:
			// Sexy old school font that will be used as fallback value when rendering
			// child elements that contain text, but doesn't have any specific font
			static const unscii16Font defaultFont;

			explicit Application(
				ScreenBuffer* screenBuffer
			);

			virtual void setup();

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation value) const;

			void tick() override;
			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;

			void handleEvent(InputEvent &event) override;
			void startAnimation(Animation* animation) override;

			Element* getCapturedElement() const;
			void setCapturedElement(Element* capturedElement);

			Element* getFocusedElement() const;
			void setFocusedElement(Element* focusedElement);

			const Font* getFont() const;
			void setFont(const Font* font);

			ScreenBuffer* getScreenBuffer() const;

			void addInputDevice(InputDevice* inputDevice);

			Callback<>& getOnTick();

		private:
			ScreenBuffer* _screenBuffer;

			bool _isRendered = false;
			bool _isMeasuredAndArranged = false;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			Callback<> _onTick {};

			const Font* _defaultFont = &defaultFont;

			void animationsTick();
	};
}