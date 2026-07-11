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
			Element* getChild() const {
				return _child;
			}

			void setChild(Element* child) {
				if (child == _child)
					return;

				if (_child)
					onChildRemoved(_child);

				_child = child;

				if (_child)
					onChildAdded(child);
			}

			Decorator& operator+=(Element* child) {
				setChild(child);

				return *this;
			}

			Decorator& operator-=(Element*) {
				setChild(nullptr);

				return *this;
			}

		protected:
			void handleEvent(Event* event, const Rectangle& parentBounds, bool callHandlers) override;

			void onTick() override {
				if (!_child)
					return;

				_child->onTick();
			}

			Size onMeasure(const Size& availableSize) override {
				if (!_child || !_child->isVisible())
					return {};

				_child->measure(availableSize);

				return _child->getMeasuredSize();
			}

			void onArrange(const Rectangle& bounds) override {
				if (!_child || !_child->isVisible())
					return;

				_child->arrange(bounds);
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				if (!_child || !_child->isVisible())
					return;

				_child->onRender(renderer, bounds);
			}

			virtual void onEventBeforeChild(Event* event) {

			}

			virtual void onEventAfterChild(Event* event) {

			}

		private:
			Element* _child = nullptr;
	};
}