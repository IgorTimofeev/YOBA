#pragma once

#include <vector>
#include <YOBA/UI/Element.hpp>
#include <YOBA/UI/Parent.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Rectangle.hpp>
#include <YOBA/Core/Size.hpp>
#include <YOBA/Core/Events/Event.hpp>
#include <iterator>

namespace YOBA {
	class Layout : public virtual Parent {
		public:
			size_t getChildrenCount() const;
			bool hasChildren() const;

			size_t getIndexOfChild(Element* element);
			Element* getChildAt(size_t index) const;

			void addChild(Element* child);
			void insertChild(size_t index, Element* child);
			void insertChildFromEnd(size_t offset, Element* child);

			void removeChildAt(int index);
			void removeChild(Element* child);
			void removeChildren();
			void moveChildrenTo(Layout* layout);
			void removeAndDeleteChildren();

			std::vector<Element*>::iterator begin();
			std::vector<Element*>::iterator end();

			Element* operator[](size_t index) const;
			Layout& operator+=(Element* child);
			Layout& operator-=(Element* child);

		protected:
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

			virtual void onEventBeforeChildren(Event* event);
			virtual void onEventAfterChildren(Event* event);

		private:
			std::vector<Element*> _children {};

			// Moving to private
			void onParentHandleEventBeforeChildren(Event* event) override;
			void onParentHandlePointerEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) override;
			void onParentHandleEventForChildren(Event* event, const Rectangle& bounds, bool callHandlers) override;
			void onParentHandleEventAfterChildren(Event* event) override;
	};
}