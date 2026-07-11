#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/UI/Parent.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Rectangle.hpp>
#include <YOBA/Core/Size.hpp>
#include <YOBA/Core/Events/Event.hpp>

namespace YOBA {
	class Decorator : public virtual Parent {
		public:
			Element* getChild() const;
			void setChild(Element* child);

			bool hasChild() const;

			Parent& operator+=(Element* child) override;
			Parent& operator-=(Element*) override;

		protected:
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

			virtual void onEventBeforeChild(Event* event);
			virtual void onEventAfterChild(Event* event);

		private:
			Element* _child = nullptr;

			// Moving to private
			void onParentHandleEventBeforeChildren(Event* event) override;
			void onParentHandlePointerEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) override;
			void onParentHandleEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) override;
			void onParentHandleEventAfterChildren(Event* event) override;
	};
}