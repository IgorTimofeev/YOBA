#pragma once

#include <vector>
#include <YOBA/UI/element.h>
#include <YOBA/main/rendering/renderer.h>
#include <YOBA/main/bounds.h>
#include <YOBA/main/size.h>
#include <YOBA/main/events/event.h>
#include <iterator>

namespace YOBA {
	// Вложенный класс итератора
	class LayoutIterator {
		public:
			LayoutIterator(const std::vector<Element*>::iterator vectorIterator) : _vectorIterator(vectorIterator) {

			}

			Element& operator*() const {
				return **_vectorIterator;
			}

			Element* operator->() const {
				return *_vectorIterator;
			}

			LayoutIterator& operator++() {
				++_vectorIterator;

				return *this;
			}

			LayoutIterator operator++(int) {
				const LayoutIterator tmp = *this;
				++_vectorIterator;
				return tmp;
			}

			bool operator==(const LayoutIterator& other) const {
				return _vectorIterator == other._vectorIterator;
			}

			bool operator!=(const LayoutIterator& other) const {
				return _vectorIterator != other._vectorIterator;
			}

		private:
			std::vector<Element*>::iterator _vectorIterator;
	};

	class Layout : public virtual Element {
		public:
			size_t getChildrenCount() const;
			size_t getIndexOfChild(Element& element);
			Element& getChildAt(size_t index) const;

			void addChild(Element& child);
			void insertChild(size_t index, Element& child);
			void insertChildFromEnd(size_t offset, Element& child);

			void removeChildAt(int index);
			void removeChild(Element& child);
			void removeChildren();
			void moveChildrenTo(Layout& layout);
			void removeAndDeleteChildren();

			LayoutIterator begin();
			LayoutIterator end();

			Element& operator[](size_t index) const;
			Layout& operator+=(Element& child);
			Layout& operator-=(Element& child);

		protected:
			void handleEvent(Event* event, const Bounds& parentBounds, bool callHandlers) override;
			void onTick() override;
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;

			virtual void onEventBeforeChildren(Event* event);
			virtual void onEventAfterChildren(Event* event);

			virtual void onChildAdded(Element& child);
			virtual void onChildRemoved(Element& child);

		private:
			std::vector<Element*> _children {};
	};
}