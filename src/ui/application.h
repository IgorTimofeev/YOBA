#pragma once

#include "layout.h"
#include "hardware/inputDevice.h"
#include "font.h"
#include "../resources/fonts/unscii16Font.h"
#include "rendering/renderer.h"

namespace yoba::ui {
	class Application : public Layout {
		public:
			explicit Application(Renderer* renderer);

			virtual void setup();
			virtual void tick();

			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;

			void handleEvent(Event& event) override;
			void startAnimation(Animation* animation) override;

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

			void animationsTick();
			void inputDevicesTick();
	};
}