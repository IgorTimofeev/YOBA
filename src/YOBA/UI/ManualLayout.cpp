#include <algorithm>

#include <YOBA/UI/ManualLayout.hpp>

namespace YOBA {
	Point ManualLayout::getChildPosition(const Element* child) {
		return _childPositions.contains(child) ? _childPositions[child] : Point();
	}

	void ManualLayout::setChildPosition(const Element* child, const Point& value) {
		if (value.getX() == 0 && value.getY() == 0) {
			_childPositions.erase(child);
		}
		else {
			_childPositions.insert_or_assign(child, value);
		}
	}

	Size ManualLayout::onMeasure(const Size& availableSize) {
		uint16_t maxWidth = 0;
		uint16_t maxHeight = 0;
		Point childPosition;

		// 1st loop, computing sum of relative sizes & sum of auto sizes
		for (const auto child: *this) {
			if (!child->isVisible())
				continue;

			childPosition = getChildPosition(child);
			child->measure(availableSize);

			maxWidth = std::max<uint16_t>(maxWidth, childPosition.getX() + child->getMeasuredSize().getWidth() - 1);
			maxHeight = std::max<uint16_t>(maxHeight, childPosition.getY() + child->getMeasuredSize().getHeight() - 1);
		}

		return Size(maxWidth, maxHeight);
	}

	void ManualLayout::onArrange(const Rectangle& bounds) {
		Point childPosition;
		Size childSize;

		for (const auto child: *this) {
			if (!child->isVisible())
				continue;

			childPosition = getChildPosition(child);
			childSize = getMeasuredSize();

			child->arrange(Rectangle(
				bounds.getX() + childPosition.getX(),
				bounds.getY() + childPosition.getY(),
				childSize.getWidth(),
				childSize.getHeight()
			));

			auto a = 123;
		}
	}

	void ManualLayout::onChildRemoved(Element* child) {
		Layout::onChildRemoved(child);

		_childPositions.erase(child);
	}
}
