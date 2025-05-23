#pragma once

#include "layout.h"
#include <YOBA/main/font.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/UI/traits/backgroundColorElement.h>
#include <YOBA/hardware/HID.h>

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
			// hid.setup();
			// application.addHID(&hid);
			void setup(RenderTarget* renderTarget, Renderer* renderer, HID* hid);

			virtual void tick();
			virtual void render();

			void invalidateMeasure() override;
			void invalidateRender() override;
			void invalidate() override;

			void handleEvent(Event* event) override;
			void startAnimation(Animation* animation) override;

			Renderer* getRenderer() const;
			void setRenderer(Renderer* value);

			void addHID(HID* hid);

			void scheduleOnTick(const std::function<void()>& task);

			uint32_t getTickDeltaTime() const;
			uint32_t getLayoutDeltaTime() const;
			uint32_t getRenderDeltaTime() const;
			uint32_t getFlushDeltaTime() const;
			uint32_t getPeripheralsDeltaTime() const;

		protected:
			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			static Application* _current;
			Renderer* _renderer = nullptr;

			bool _renderInvalidated = true;
			bool _measureInvalidated = true;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			std::vector<HID*> _HIDs {};
			std::vector<std::function<void()>> _scheduledOnTickTasks {};

			uint32_t _peripheralsDeltaTime = 0;
			uint32_t _tickDeltaTime = 0;
			uint32_t _layoutDeltaTime = 0;
			uint32_t _renderDeltaTime = 0;
			uint32_t _flushDeltaTime = 0;

			void animationsTick();

			// Will be only called from Element
			Element* getFocusedElement() const;
			void setFocusedElement(Element* element);

			Element* getCapturedElement() const;
			void setCapturedElement(Element* element);
	};
}