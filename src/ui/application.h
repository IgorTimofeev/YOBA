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

			const Font* getFont() const;
			void setFont(const Font* font);

			ScreenBuffer* getScreenBuffer() const;

			void addInputDevice(InputDevice* inputDevice);

			Element* getCapturedElement() const;
			void setCapturedElement(Element* element);

			Element* getFocusedElement() const;
			void setFocusedElement(Element* element);

			void enqueueOnTick(const std::function<void()>& task);

		private:
			ScreenBuffer* _screenBuffer;

			bool _isRendered = false;
			bool _isMeasuredAndArranged = false;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			std::vector<InputDevice*> _inputDevices {};
			std::vector<std::function<void()>> _enqueuedTasksOnTick {};

			const Font* _defaultFont = &defaultFont;

			void animationsTick();
	};
}