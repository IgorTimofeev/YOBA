#include "container.h"
#include "event.h"

namespace yoba::ui {
	void Container::tick() {
		Element::tick();

		for (auto element : _children)
			element->tick();
	}

	void Container::onRender(Renderer* renderer) {
		for (auto element : _children)
			element->render(renderer);

		Element::onRender(renderer);
	}

	void Container::onEvent(InputEvent &event) {
		if (getChildrenCount() == 0)
			return;

		size_t i = getChildrenCount() - 1;

		while (true) {
			(*this)[i]->handleEvent(event);

			if (event.isHandled() || i == 0)
				return;

			i--;
		}
	}


	size_t Container::getChildrenCount() {
		return _children.size();
	}

	int32_t Container::getIndexOfChild(Element* element) {
		auto iterator = find(_children.begin(), _children.end(), element);

		if (iterator == _children.end()) {
			return -1;
		}
		else {
			return iterator - _children.begin();
		}
	}

	void Container::removeChildAt(int index) {
		_children.erase(_children.begin() + index);

		invalidate();
	}

	void Container::removeChild(Element* child) {
		auto iterator = std::find(_children.begin(), _children.end(), child);

		if (iterator == _children.end())
			return;

		_children.erase(iterator);

		child->onRemovedFromParent(this);

		invalidate();
	}

	void Container::removeChildren() {
		for (auto child : _children) {
			child->onRemovedFromParent(this);
		}

		_children.clear();

		invalidate();
	}

	Element* Container::getChildAt(size_t index) {
		return _children[index];
	}

	void Container::addChild(Element* child) {
		_children.push_back(child);

		child->onAddedToParent(this);

		invalidate();
	}

	Element* Container::operator[](size_t index) {
		return getChildAt(index);
	}

	void Container::operator+=(Element* child) {
		addChild(child);
	}

	void Container::operator-=(Element* child) {
		removeChild(child);
	}

	Size Container::onMeasure(Renderer* renderer, const Size& availableSize) {
		auto result = Size();

		for (auto child : _children) {
			if (!child->isVisible())
				continue;

			child->measure(renderer, availableSize);

			if (child->getMeasuredSize().getWidth() > result.getWidth())
				result.setWidth(child->getMeasuredSize().getWidth());

			if (child->getMeasuredSize().getHeight() > result.getHeight())
				result.setHeight(child->getMeasuredSize().getHeight());
		}

		return result;
	}

	void Container::onArrange(const Bounds& bounds) {
		for (auto child : _children) {
			if (child->isVisible()) {
				child->arrange(bounds);
			}
		}
	}

	std::vector<Element*>::iterator Container::begin() {
		return _children.begin();
	}

	std::vector<Element*>::iterator Container::end() {
		return _children.end();
	}

	void Container::setApplication(Application* value) {
		Element::setApplication(value);

		for (auto child : _children)
			child->setApplication(value);
	}
}