#pragma once

#include <YOBA/rendering/renderer.h>
#include <YOBA/UI/layout.h>
#include <YOBA/UI/traits/backgroundColorElement.h>
#include <YOBA/hardware/HID.h>

namespace YOBA {
	class Application : public Layout, public BackgroundColorElement {
		friend class Element;
		friend class Control;
		friend class Layout;

		public:
			static Application* getCurrent();

			explicit Application();

			virtual void tick();
			virtual void render();

			void invalidateLayout() override;
			void invalidateRender() override;
			void invalidate() override;
			void pushEvent(Event* event);

			void addAnimation(Animation* animation);
			void removeAnimation(Animation* animation);

			Renderer* getRenderer() const;
			void setRenderer(Renderer* value);

			void addHID(HID* hid);

			void invokeLater(const std::function<void()>& func);

			uint32_t getTickDeltaTime() const;
			uint32_t getLayoutDeltaTime() const;
			uint32_t getRenderDeltaTime() const;
			uint32_t getFlushDeltaTime() const;
			uint32_t getHIDTickDeltaTime() const;

			Element* getFocusedElement() const;
			void setFocusedElement(Element* element);

			Element* getCapturedElement() const;
			void setCapturedElement(Element* element);

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			static Application* _current;
			Renderer* _renderer = nullptr;

			bool _layoutInvalidated = true;
			bool _renderInvalidated = true;
			Element* _capturedElement = nullptr;
			Element* _focusedElement = nullptr;

			std::vector<Animation*> _animations {};
			std::vector<HID*> _HIDs {};
			std::vector<std::function<void()>> _functionsToInvokeLater {};

			uint32_t _HIDTickDeltaTime = 0;
			uint32_t _tickDeltaTime = 0;
			uint32_t _layoutDeltaTime = 0;
			uint32_t _renderDeltaTime = 0;
			uint32_t _flushDeltaTime = 0;
	};
}