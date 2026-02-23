#pragma once

#include <YOBA/main/rendering/renderer.h>
#include <YOBA/UI/layout.h>
#include <YOBA/UI/traits/backgroundColorElement.h>
#include <YOBA/hardware/HID.h>

namespace YOBA {
	class Application : public Layout, public BackgroundColorElement {
		friend class Element;
		friend class Control;
		friend class Layout;

		public:
			static bool hasCurrent();
			static Application& getCurrent();

			explicit Application();

			// Shorthand method for convenient initialization, and it's equivalent to:
			//
			// renderTarget.setup();
			// renderer.setTarget(&renderTarget);
			// application.setRenderer(&renderer);
			void setup(RenderTarget& renderTarget, Renderer& renderer);

			// Shorthand method for convenient initialization, and it's equivalent to:
			//
			// renderTarget.setup();
			// renderer.setTarget(&renderTarget);
			// application.setRenderer(&renderer);
			//
			// application.addHID(&hid);
			void setup(RenderTarget& renderTarget, Renderer& renderer, HID& hid);

			virtual void tick();
			virtual void render();

			void invalidateMeasure() override;
			void invalidateRender() override;
			void invalidate() override;
			void pushEvent(Event* event);

			void startAnimation(Animation* animation) override;

			Renderer& getRenderer() const;
			void setRenderer(Renderer& value);

			void addHID(HID& hid);

			void invokeOnNextTick(const std::function<void()>& func);

			void requestSecondRenderPass();

			uint32_t getTickDeltaTime() const;
			uint32_t getLayoutDeltaTime() const;
			uint32_t getRenderDeltaTime() const;
			uint32_t getFlushDeltaTime() const;
			uint32_t getHIDTickDeltaTime() const;

			bool hasFocusedElement() const;
			Element& getFocusedElement() const;
			void clearElementFocus();
			void focusElement(Element& element);

			bool hasCapturedElement() const;
			Element& getCapturedElement() const;
			void releaseElementCapture();
			void captureElement(Element& element);

		protected:
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			static Application* _current;
			Renderer* _renderer = nullptr;

			bool _secondRenderPassRequested = false;
			bool _renderInvalidated = true;
			bool _measureInvalidated = true;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			std::vector<HID*> _HIDs {};
			std::vector<std::function<void()>> _functionToCallOnNextTick {};

			uint32_t _HIDTickDeltaTime = 0;
			uint32_t _tickDeltaTime = 0;
			uint32_t _layoutDeltaTime = 0;
			uint32_t _renderDeltaTime = 0;
			uint32_t _flushDeltaTime = 0;

			void setCapturedElementInternal(Element* element);
			void setFocusedElementInternal(Element* element);

			void animationsTick();
	};
}