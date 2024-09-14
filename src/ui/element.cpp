#include "element.h"
#include "workspace.h"
#include "animation.h"
#include "hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	bool Element::isVisible() const {
		return _isVisible;
	}

	void Element::setVisible(bool value) {
		_isVisible = value;

		invalidate();
	}

	bool Element::isEnabled() const {
		return _isEnabled;
	}

	void Element::setEnabled(bool value) {
		_isEnabled = value;

		invalidate();
	}

	void Element::calculateMeasureShit(const Alignment &alignment, const uint16_t &size,
									   const uint16_t &desiredSize, const int32_t &marginStart, const int32_t &marginEnd, int32_t &newSize) {
		if (size == Size::calculated) {
			newSize = desiredSize;
		}
		else {
			newSize = size;
		}

		newSize = newSize + marginStart + marginEnd;

		if (newSize < 0)
			newSize = 0;
	}

	Size Element::measure(ScreenBuffer* screenBuffer, const Size &availableSize) {
		auto desiredSize = onMeasure(screenBuffer, availableSize);

		auto& size = getSize();
		auto& margin = getMargin();

		int32_t newSize = 0;

		// Width
		calculateMeasureShit(
			getHorizontalAlignment(),
			size.getWidth(),
			desiredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			newSize
		);

		desiredSize.setWidth(newSize);

		// Height
		calculateMeasureShit(
			getVerticalAlignment(),
			size.getHeight(),
			desiredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			newSize
		);

		desiredSize.setHeight(newSize);

		setDesiredSize(desiredSize);

		return desiredSize;
	}

	void Element::calculateArrangeShit(
		const Alignment &alignment,
		const int32_t &position,
		const uint16_t &size,

		const uint16_t &desiredSize,
		const int32_t &marginStart,
		const int32_t &marginEnd,

		const uint16_t &limit,
		int32_t &newPosition,
		int32_t &newSize
	) {
		switch (alignment) {
			case start:
				if (size == Size::calculated) {
					newSize = desiredSize;
				}
				else {
					newSize = size;
				}

				newSize = newSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;

			case center:
				if (size == Size::calculated) {
					newSize = desiredSize;
				}
				else {
					newSize = size;
				}

				newSize = newSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart - marginEnd + limit / 2 - newSize / 2;

				break;

			case end:
				if (size == Size::calculated) {
					newSize = desiredSize;
				}
				else {
					newSize = size;
				}

				newSize = newSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + limit - marginEnd - newSize;

				break;

			case stretch:
				if (size == Size::calculated) {
					newSize = limit;
				}
				else {
					newSize = size;
				}

				newSize = newSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;
		}
	}

	void Element::arrange(const Bounds &bounds) {
		auto& margin = getMargin();
		auto& desiredSize = getDesiredSize();
		auto& size = getSize();

		Bounds newBounds;
		int32_t newPosition = 0;
		int32_t newSize = 0;

		calculateArrangeShit(
			getHorizontalAlignment(),
			bounds.getX(),
			size.getWidth(),
			desiredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			bounds.getWidth(),
			newPosition,
			newSize
		);

		newBounds.setX(newPosition);
		newBounds.setWidth(newSize);

		calculateArrangeShit(
			getVerticalAlignment(),
			bounds.getY(),
			size.getHeight(),
			desiredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			bounds.getHeight(),
			newPosition,
			newSize
		);
//
//		if (tag == 1) {
//			Serial.print(desiredSize.getHeight());
//			Serial.print(" x ");
//			Serial.print(bounds.getHeight());
//			Serial.print(" x ");
//			Serial.print(newPosition);
//			Serial.print(" x ");
//			Serial.println(newSize);
//		}

		newBounds.setY(newPosition);
		newBounds.setHeight(newSize);

		setBounds(newBounds);

		onArrange(newBounds);
	}

	void Element::setDesiredSize(const Size &value) {
		_desiredSize = value;
	}

	void Element::setBounds(const Bounds &value) {
		_bounds = value;
	}

	void Element::onArrange(const Bounds &bounds) {

	}

	Size Element::onMeasure(ScreenBuffer* screenBuffer, const Size &availableSize) {
		return {
			0,
			0,
		};
	}

	void Element::handleEvent(Event &event) {
		if (!event.matches(this))
			return;

		onEvent(event);

		_eventHandlers.call(event);
	}

	void Element::onEvent(Event &event) {
		event.setHandled(true);
	}

	void Element::addEventHandler(const std::function<void(Event &)> &handler) {
		_eventHandlers.add(handler);
	}

	bool Element::isCaptured() {
		return getWorkspace() && getWorkspace()->getCapturedElement() == this;
	}

	void Element::setCaptured(const bool& value) {
		if (getWorkspace())
			getWorkspace()->setCapturedElement(value ? this : nullptr);
	}

	void Element::startAnimation(Animation* animation) {
		if (_workspace)
			_workspace->startAnimation(animation);
	}

	void Element::setParent(Element *value) {
		_parent = value;
	}

	Element *Element::getParent() {
		return _parent;
	}

	void Element::setWorkspace(Workspace *value) {
		_workspace = value;
	}

	Workspace *Element::getWorkspace() {
		return _workspace;
	}

	const Bounds &Element::getBounds() {
		return _bounds;
	}

	const Size &Element::getDesiredSize() {
		return _desiredSize;
	}

	void Element::setSize(const Size &value) {
		_size = value;

		invalidate();
	}

	const Size &Element::getSize() {
		return _size;
	}

	void Element::setMargin(const Margin &value) {
		_margin = value;

		invalidate();
	}

	const Margin &Element::getMargin() {
		return _margin;
	}

	void Element::setAlignment(Alignment uniformValue) {
		setAlignment(uniformValue, uniformValue);
	}

	void Element::setAlignment(Alignment horizontal, Alignment vertical) {
		_horizontalAlignment = horizontal;
		_verticalAlignment = vertical;

		invalidate();
	}

	void Element::setVerticalAlignment(Alignment value) {
		_verticalAlignment = value;

		invalidate();
	}

	Alignment Element::getVerticalAlignment() const {
		return _verticalAlignment;
	}

	void Element::setHorizontalAlignment(Alignment value) {
		_horizontalAlignment = value;

		invalidate();
	}

	Alignment Element::getHorizontalAlignment() const {
		return _horizontalAlignment;
	}

	void Element::invalidateRender() {
		if (_workspace)
			_workspace->invalidateRender();
	}

	void Element::invalidateLayout() {
		if (_workspace)
			_workspace->invalidateLayout();
	}

	void Element::invalidate() {
		if (_workspace)
			_workspace->invalidate();
	}

	void Element::tick() {

	}

	void Element::render(ScreenBuffer* screenBuffer) {
		if (!isVisible())
			return;

		if (_clipToBounds) {
			screenBuffer->setViewport(getBounds());
		}
		else {
			screenBuffer->resetViewport();
		}

		onRender(screenBuffer);
	}

	void Element::onRender(ScreenBuffer* screenBuffer) {

	}

	bool Element::getClipToBounds() const {
		return _clipToBounds;
	}

	void Element::setClipToBounds(bool value) {
		_clipToBounds = value;
	}
}