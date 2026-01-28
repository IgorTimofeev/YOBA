#pragma once

#include <YOBA/UI/scrollBar.h>
#include <YOBA/UI/layout.h>
#include <YOBA/main/events/scrollIntoViewEvent.h>

namespace YOBA {
	enum class ScrollMode : uint8_t {
		computed,
		enabled,
		hidden,
		disabled
	};

	class ScrollView : public Layout {
		public:
			ScrollView() {
				setClipToBounds(true);

				_horizontalScrollBar.setOrientation(Orientation::horizontal);
				_horizontalScrollBar.setAlignment(Alignment::stretch, Alignment::end);

				_verticalScrollBar.setOrientation(Orientation::vertical);
				_verticalScrollBar.setAlignment(Alignment::end, Alignment::stretch);

				setScrollBarSize(3);
				setScrollBarOffset(3);
			}

			ScrollMode getHorizontalScrollMode() const {
				return _horizontalScrollMode;
			}

			void setHorizontalScrollMode(const ScrollMode value) {
				_horizontalScrollMode = value;

				invalidate();
			}

			ScrollMode getVerticalScrollMode() const {
				return _verticalScrollMode;
			}

			void setVerticalScrollMode(const ScrollMode value) {
				_verticalScrollMode = value;

				invalidate();
			}

			uint16_t getHorizontalPosition() const {
				return _horizontalScrollBar.getPosition();
			}

			void setHorizontalPosition(const uint16_t value) {
				if (!_horizontalScrollPossible)
					return;

				_horizontalScrollBar.setPosition(value);

				invalidate();
			}

			uint16_t getVerticalPosition() const {

				return _verticalScrollBar.getPosition();
			}

			void setVerticalPosition(const uint16_t value) {
				if (!_verticalScrollPossible)
					return;

				_verticalScrollBar.setPosition(value);

				invalidate();
			}

			void setScrollBarOffset(const uint16_t value) {
				_horizontalScrollBar.setMargin(Margin(value, 0, value, value));
				_verticalScrollBar.setMargin(Margin(0, value, value, value));
			}

			void setScrollBarSize(const uint16_t value) {
				_horizontalScrollBar.setSize(Size(Size::computed, value));
				_verticalScrollBar.setSize(Size(value, Size::computed));
			}

			void setScrollBarCornerRadius(const uint16_t value) {
				_horizontalScrollBar.setCornerRadius(value);
				_verticalScrollBar.setCornerRadius(value);
			}

			void setScrollBarThumbColor(const Color* value) {
				_horizontalScrollBar.setThumbColor(value);
				_verticalScrollBar.setThumbColor(value);
			}

			const Bounds& getContentBounds() const {
				return _contentBounds;
			}

			void scrollTo(const Element* element) {
				const auto& elementBounds = element->getBounds();
				const auto& bounds = getBounds();

				// Horizontal
				if (elementBounds.getX() < bounds.getX()) {
					scrollHorizontallyBy(elementBounds.getX() - bounds.getX());
				}
				else if (elementBounds.getX2() > bounds.getX2()) {
					scrollHorizontallyBy(elementBounds.getX2() - bounds.getX2());
				}

				// Vertical
				if (elementBounds.getY() < bounds.getY()) {
					scrollVerticallyBy(elementBounds.getY() - bounds.getY());
				}
				else if (elementBounds.getY2() > bounds.getY2()) {
					scrollVerticallyBy(elementBounds.getY2() - bounds.getY2());
				}
			}

			void scrollToCenter(const Element* element) {
				const auto& elementBounds = element->getBounds();
				const auto& bounds = getBounds();

				const auto elementCenter = elementBounds.getCenter();
				const auto center = bounds.getCenter();

				// Horizontal
				scrollHorizontallyBy(elementCenter.getX() - center.getX());
				scrollVerticallyBy(elementCenter.getY() - center.getY());
			}

		protected:
			Size onMeasure(const Size& availableSize) override {
				const auto& contentSize = Size(
					_horizontalScrollMode == ScrollMode::disabled
					? availableSize.getWidth()
					: Size::unlimited,

					_verticalScrollMode == ScrollMode::disabled
					? availableSize.getHeight()
					: Size::unlimited
				);

				_contentMeasuredSize.setWidth(0);
				_contentMeasuredSize.setHeight(0);

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->measure(contentSize);

					const auto& elementSize = element->getMeasuredSize();

					if (elementSize.getWidth() > _contentMeasuredSize.getWidth())
						_contentMeasuredSize.setWidth(elementSize.getWidth());

					if (elementSize.getHeight() > _contentMeasuredSize.getHeight())
						_contentMeasuredSize.setHeight(elementSize.getHeight());
				}

				_horizontalScrollBar.measure(availableSize);
				_verticalScrollBar.measure(availableSize);

				return _contentMeasuredSize;
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				if (_scrollLaterTo) {
					// FR FR 100% need to scroll
					if (!bounds.contains(_scrollLaterTo->getBounds()))
						scrollToCenter(_scrollLaterTo);

					_scrollLaterTo = nullptr;
				}

				if (_horizontalScrollMode == ScrollMode::disabled) {
					if (_horizontalScrollBar.getPosition() > 0)
						_horizontalScrollBar.setPosition(0);

					_contentBounds.setX(bounds.getX());
					_contentBounds.setWidth(bounds.getWidth());
				}
				else {
					if (_horizontalScrollBar.getPosition() > 0) {
						if (_contentMeasuredSize.getWidth() > bounds.getWidth()) {
							if (bounds.getX() + _contentMeasuredSize.getWidth() - 1 - _horizontalScrollBar.getPosition() < bounds.getX2())
								_horizontalScrollBar.setPosition(_contentMeasuredSize.getWidth() - bounds.getWidth());
						}
						else {
							_horizontalScrollBar.setPosition(0);
						}
					}

					_contentBounds.setX(bounds.getX() - _horizontalScrollBar.getPosition());
					_contentBounds.setWidth(_contentMeasuredSize.getWidth());
				}

				if (_verticalScrollMode == ScrollMode::disabled) {
					if (_verticalScrollBar.getPosition() > 0)
						_verticalScrollBar.setPosition(0);

					_contentBounds.setY(bounds.getY());
					_contentBounds.setHeight(bounds.getHeight());
				}
				else {
					if (_verticalScrollBar.getPosition() > 0) {
						// If content height > scroll view height
						if (_contentMeasuredSize.getHeight() > bounds.getHeight()) {
							// If content Y2 with position < scroll view Y2
							if (bounds.getY() + _contentMeasuredSize.getHeight() - 1 - _verticalScrollBar.getPosition() < bounds.getY2())
								_verticalScrollBar.setPosition(_contentMeasuredSize.getHeight() - bounds.getHeight());
						}
						else {
							_verticalScrollBar.setPosition(0);
						}
					}

					_contentBounds.setY(bounds.getY() - _verticalScrollBar.getPosition());
					_contentBounds.setHeight(_contentMeasuredSize.getHeight());
				}

				for (const auto element : *this)
					if (element->isVisible())
						element->render(renderer, _contentBounds);

				const auto& processScrollBar = [&bounds, &renderer](ScrollBar& bar, bool& possible, const ScrollMode mode, const uint16_t contentSize, const uint16_t viewportSize) {
					bar.setContentSize(contentSize);
					bar.setViewportSize(viewportSize);
					possible = mode != ScrollMode::disabled && contentSize > viewportSize;

					bar.setVisible(
						mode == ScrollMode::enabled
						|| (mode == ScrollMode::computed && possible)
					);

					if (bar.isVisible())
						bar.render(renderer, bounds);
				};

				processScrollBar(
					_horizontalScrollBar,
					_horizontalScrollPossible,
					_horizontalScrollMode,
					_contentBounds.getWidth(),
					bounds.getWidth()
				);

				processScrollBar(
					_verticalScrollBar,
					_verticalScrollPossible,
					_verticalScrollMode,
					_contentBounds.getHeight(),
					bounds.getHeight()
				);

				// ESP_LOGI("SCROLL", "contentSize: %f, viewportSize: %f", (float) contentBounds.getHeight(), (float) bounds.getHeight());
			}

			void onCaptureChanged() override {
				Layout::onCaptureChanged();

				if (!isCaptured())
					_lastTouchPosition.setX(-1);
			}

			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					_lastTouchPosition = reinterpret_cast<PointerDownEvent*>(event)->getPosition();
				}
				else if (event->getTypeID() == PointerDragEvent::typeID) {
					if (_lastTouchPosition.getX() >= 0) {
						const auto position = reinterpret_cast<PointerDragEvent*>(event)->getPosition();
						const auto pointerDelta = position - _lastTouchPosition;

						if (isCaptured()) {
							_lastTouchPosition = position;

							scrollHorizontallyBy(-pointerDelta.getX());
							scrollVerticallyBy(-pointerDelta.getY());
						}
						else {
							if (pointerDelta.getLength() >= 3) {
								_lastTouchPosition = position;

								setCaptured(true);
							}
						}
					}
				}
				else if (event->getTypeID() == ScrollIntoViewEvent::typeID) {
					_scrollLaterTo = reinterpret_cast<ScrollIntoViewEvent*>(event)->getElement();

					event->setHandled(true);
				}
			}

			void onEventAfterChildren(Event* event) override {
				if (event->getTypeID() == PointerUpEvent::typeID) {
					setCaptured(false);
				}
			}

			void scrollHorizontallyBy(const int32_t delta) {
				setHorizontalPosition(static_cast<uint16_t>(std::max(
					static_cast<int32_t>(getHorizontalPosition()) + delta,
					static_cast<int32_t>(0)
				)));
			}

			void scrollVerticallyBy(const int32_t delta) {
				setVerticalPosition(static_cast<uint16_t>(std::max(
					static_cast<int32_t>(getVerticalPosition()) + delta,
					static_cast<int32_t>(0)
				)));
			}

		private:
			ScrollBar _horizontalScrollBar {};
			ScrollBar _verticalScrollBar {};

			ScrollMode _horizontalScrollMode = ScrollMode::disabled;
			ScrollMode _verticalScrollMode = ScrollMode::computed;

			Size _contentMeasuredSize {};
			Bounds _contentBounds {};
			const Element* _scrollLaterTo = nullptr;

			bool _horizontalScrollPossible = false;
			bool _verticalScrollPossible = false;

			Point _lastTouchPosition { -1, -1 };
	};
}