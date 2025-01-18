#include "element.h"
#include "layout.h"
#include "application.h"
#include "animation.h"
#include "event.h"

namespace yoba::ui {
	Element::~Element() {
		if (isCaptured())
			setCaptured(false);
	}

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

	Size Element::onMeasure(const Size& availableSize) {
		return { 0, 0 };
	}

	void Element::computeMeasureShit(
		const uint16_t &size,
		const uint16_t &desiredSize,
		const int32_t &marginStart,
		const int32_t &marginEnd,
		int32_t &newSize
	) {
		if (size == Size::computed) {
			newSize = desiredSize;
		}
		else {
			newSize = size;
		}

		newSize = newSize + marginStart + marginEnd;

		if (newSize < 0)
			newSize = 0;
	}

	void Element::onTick() {

	}

	void Element::measure(const Size& availableSize) {
		const auto& size = getSize();
		const auto& margin = getMargin();

		auto desiredSize = onMeasure(Size(
			availableSize.getWidth() - margin.getLeft() - margin.getRight(),
			availableSize.getHeight() - margin.getTop() - margin.getBottom()
		));

		int32_t newSize = 0;

		// Width
		computeMeasureShit(
			size.getWidth(),
			desiredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			newSize
		);

		desiredSize.setWidth(newSize);

		// Height
		computeMeasureShit(
			size.getHeight(),
			desiredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			newSize
		);

		desiredSize.setHeight(newSize);

		_measuredSize = desiredSize;
	}

	void Element::computeArrangeShit(
		const Alignment &alignment,
		const int32_t &position,
		const uint16_t &size,

		const uint16_t &measuredSize,
		const int32_t &marginStart,
		const int32_t &marginEnd,

		const uint16_t &limit,
		int32_t &newPosition,
		int32_t &newSize
	) {
		switch (alignment) {
			case Alignment::start:
				newSize = measuredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;

			case Alignment::center:
				newSize = measuredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart - marginEnd + limit / 2 - newSize / 2;

				break;

			case Alignment::end:
				newSize = measuredSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + limit - marginEnd - newSize;

				break;

			case Alignment::stretch:
				if (size == Size::computed) {
					newSize = limit;
				}
				else {
					newSize = measuredSize;
				}

				newSize = newSize - marginStart - marginEnd;

				if (newSize < 0)
					newSize = 0;

				newPosition = position + marginStart;

				break;
		}
	}

	void Element::onArrange(const Bounds& bounds) {

	}

	void Element::arrange(const Bounds& bounds) {
		auto& margin = getMargin();
		auto& measuredSize = getMeasuredSize();
		auto& size = getSize();

		Bounds newBounds;
		int32_t newPosition = 0;
		int32_t newSize = 0;

		computeArrangeShit(
			getHorizontalAlignment(),
			bounds.getX(),
			size.getWidth(),
			measuredSize.getWidth(),
			margin.getLeft(),
			margin.getRight(),
			bounds.getWidth(),
			newPosition,
			newSize
		);

		newBounds.setX(newPosition);
		newBounds.setWidth(newSize);

		computeArrangeShit(
			getVerticalAlignment(),
			bounds.getY(),
			size.getHeight(),
			measuredSize.getHeight(),
			margin.getTop(),
			margin.getBottom(),
			bounds.getHeight(),
			newPosition,
			newSize
		);

		newBounds.setY(newPosition);
		newBounds.setHeight(newSize);

		_bounds = newBounds;

		onArrange(newBounds);

		onBoundsChanged();
	}

	void Element::handleEvent(Event& event) {
		if (!event.matches(this))
			return;

		onEvent(event);
	}

	void Element::onEvent(Event& event) {

	}

	bool Element::isCaptured() {
		return _application && _application->getCapturedElement() == this;
	}

	void Element::setCaptured(bool value) {
		if (_application)
			_application->setCapturedElement(value ? this : nullptr);
	}

	void Element::startAnimation(Animation* animation) {
		if (_application)
			_application->startAnimation(animation);
	}

	Element* Element::getParent() {
		return _parent;
	}

	void Element::setApplication(Application* value) {
		_application = value;
	}

	Application *Element::getApplication() {
		return _application;
	}

	const Bounds &Element::getBounds() {
		return _bounds;
	}

	const Size& Element::getMeasuredSize() {
		return _measuredSize;
	}

	void Element::setSize(const Size& value) {
		_size = value;

		invalidate();
	}

	const Size& Element::getSize() {
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
		if (_application)
			_application->invalidateRender();
	}

	void Element::invalidateLayout() {
		if (_application)
			_application->invalidateLayout();
	}

	void Element::invalidate() {
		if (_application)
			_application->invalidate();
	}

	void Element::render(Renderer* renderer) {
		if (!isVisible())
			return;

		if (_clipToBounds) {
			auto viewport = renderer->pushViewport(getBounds());
			onRender(renderer);
			renderer->popViewport(viewport);
		}
		else {
			onRender(renderer);
		}
	}

	void Element::onRender(Renderer* renderer) {

	}

	bool Element::getClipToBounds() const {
		return _clipToBounds;
	}

	void Element::setClipToBounds(bool value) {
		_clipToBounds = value;
	}

	void Element::setWidth(uint16_t value) {
		_size.setWidth(value);

		invalidate();
	}

	void Element::setHeight(uint16_t value) {
		_size.setHeight(value);

		invalidate();
	}

	void Element::onAddedToParent(Layout* parent) {
		_parent = parent;
		setApplication(_parent->getApplication());
	}

	void Element::onRemovedFromParent(Layout* parent) {
		_parent = nullptr;
		setApplication(nullptr);
	}

	void Element::onFocusChanged() {

	}

	void Element::onCaptureChanged() {

	}

	void Element::onBoundsChanged() {

	}
}