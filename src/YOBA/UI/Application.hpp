#pragma once

#include <functional>
#include <vector>

#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/BackgroundColorElement.hpp>
#include <YOBA/Hardware/HID.hpp>

namespace YOBA {
	class Application : public Layout, public BackgroundColorElement {
		friend class Element;
		friend class Control;
		friend class Layout;

		public:
			static Application* getCurrent();

			explicit Application();

			virtual void tick();
			virtual void updateLayout();
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

			Element* getFocusedElement() const;
			void setFocusedElement(Element* element);

			Element* getCapturedElement() const;
			void setCapturedElement(Element* element);

			bool isLayoutInvalidated() const;
			bool isRenderInvalidated() const;
			bool isInvalidated() const;

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
	};
}