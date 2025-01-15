#pragma once

#include "container.h"
#include "hardware/inputDevice.h"
#include "font.h"
#include "../resources/fonts/unscii16Font.h"
#include "rendering/renderer.h"

namespace yoba::ui {
	class Application : public Container {
		public:
			// Sexy old school font that will be used as fallback value when rendering
			// child elements that contain text, but doesn't have any specific font
			static const unscii16Font defaultFont;

			explicit Application(Renderer* renderer);

			virtual void setup();

			void tick() override;
			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;

			void handleEvent(InputEvent& event) override;
			void startAnimation(Animation* animation) override;

			const Font* getFont() const;
			void setFont(const Font* font);

			Renderer* getRenderer() const;

			void addInputDevice(hardware::InputDevice* inputDevice);

			Element* getCapturedElement() const;
			void setCapturedElement(Element* element);

			Element* getFocusedElement() const;
			void setFocusedElement(Element* element);

			void enqueueOnTick(const std::function<void()>& task);

		private:
			Renderer* _renderer;

			bool _isRendered = false;
			bool _isMeasuredAndArranged = false;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			std::vector<hardware::InputDevice*> _inputDevices {};
			std::vector<std::function<void()>> _enqueuedTasksOnTick {};

			const Font* _defaultFont = &defaultFont;

			void animationsTick();
	};
}