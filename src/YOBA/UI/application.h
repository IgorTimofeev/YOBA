#pragma once

#include "layout.h"
#include "YOBA/main/font.h"
#include "YOBA/main/rendering/renderer.h"
#include "YOBA/main/inputDevice.h"
#include "YOBA/UI/traits/backgroundColorElement.h"

namespace YOBA {
	class Application : public Layout, public BackgroundColorElement {
		friend class Element;

		public:
			static Application* getCurrent();

			explicit Application();

			// Shorthand method for convenient initialization, and it's equivalent to:
			//
			// renderTarget.setup();
			// renderer->setTarget(&renderTarget);
			// application->setRenderer(&renderer);
			void setup(RenderTarget* renderTarget, Renderer* renderer);

			// Shorthand method for convenient initialization, and it's equivalent to:
			//
			// renderTarget.setup();
			// renderer->setTarget(&renderTarget);
			// application.setRenderer(&renderer);
			//
			// inputDevice.setup();
			// application.addInputDevice(&inputDevice);
			void setup(RenderTarget* renderTarget, Renderer* renderer, InputDevice* inputDevice);

			virtual void tick();

			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;

			void handleEvent(Event* event) override;
			void startAnimation(Animation* animation) override;

			Renderer* getRenderer() const;
			void setRenderer(Renderer* value);

			void addInputDevice(InputDevice* inputDevice);

			Element* getCapturedElement() const;
			Element* getFocusedElement() const;

			void scheduleTask(const std::function<void()>& task);

			uint32_t getTickDeltaTime() const;
			uint32_t getLayoutDeltaTime() const;
			uint32_t getRenderDeltaTime() const;
			uint32_t getFlushDeltaTime() const;
			uint32_t getPeripheralsDeltaTime() const;

		protected:
			void onRender(Renderer* renderer) override;

		private:
			static Application* _current;
			Renderer* _renderer = nullptr;

			bool _renderInvalidated = true;
			bool _layoutInvalidated = true;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			std::vector<InputDevice*> _inputDevices {};
			std::vector<std::function<void()>> _scheduledTasks {};

			uint32_t _peripheralsDeltaTime = 0;
			uint32_t _tickDeltaTime = 0;
			uint32_t _layoutDeltaTime = 0;
			uint32_t _renderDeltaTime = 0;
			uint32_t _flushDeltaTime = 0;

			void animationsTick();
			void inputDevicesTick();

			// Will be only called from Element
			void setFocusedElement(Element* element);
			void setCapturedElement(Element* element);
	};
}