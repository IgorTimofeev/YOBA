#pragma once

#include "scrollBar.h"
#include "layout.h"

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

				setScrollBarSize(2);
				setScrollBarSize(1);
				setScrollBarOffset(3);
			}

			ScrollMode getHorizontalScrollMode() const {
				return _horizontalScrollMode;
			}

			void setHorizontalScrollMode(ScrollMode value) {
				_horizontalScrollMode = value;

				invalidate();
			}

			ScrollMode getVerticalScrollMode() const {
				return _verticalScrollMode;
			}

			void setVerticalScrollMode(ScrollMode value) {
				_verticalScrollMode = value;

				invalidate();
			}

			uint16_t getHorizontalPosition() const {
				return _horizontalScrollBar.getPosition();
			}

			void setHorizontalPosition(uint16_t value) {
				_horizontalScrollBar.setPosition(value);

				invalidate();
			}

			uint16_t getVerticalPosition() const {
				return _verticalScrollBar.getPosition();
			}

			void setVerticalPosition(uint16_t value) {
				_verticalScrollBar.setPosition(value);

				invalidate();
			}

			void setScrollBarOffset(uint16_t value) {
				_horizontalScrollBar.setMargin(Margin(value, 0, value, value));
				_verticalScrollBar.setMargin(Margin(0, value, value, value));
			}

			void setScrollBarSize(uint16_t value) {
				_horizontalScrollBar.setSize(Size(Size::computed, value));
				_verticalScrollBar.setSize(Size(value, Size::computed));
			}

			void setScrollBarCornerRadius(uint16_t value) {
				_horizontalScrollBar.setCornerRadius(value);
				_verticalScrollBar.setCornerRadius(value);
			}

			void setScrollBarThumbColor(const Color* value) {
				_horizontalScrollBar.setThumbColor(value);
				_verticalScrollBar.setThumbColor(value);
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

				Size result = Size();

				for (auto element : *this) {
					if (!element->isVisible())
						continue;

					element->measure(contentSize);

					const auto& elementSize = element->getMeasuredSize();

					if (elementSize.getWidth() > result.getWidth())
						result.setWidth(elementSize.getWidth());

					if (elementSize.getHeight() > result.getHeight())
						result.setHeight(elementSize.getHeight());
				}

				_horizontalScrollBar.measure(availableSize);
				_verticalScrollBar.measure(availableSize);

				return result;
			}

			void onRender(Renderer* renderer) override {
				const auto& bounds = getBounds();
				const auto& measuredSize = getMeasuredSize();

				_contentBounds.setX(
					_horizontalScrollMode == ScrollMode::disabled
					? bounds.getX()
					: bounds.getX() - _horizontalScrollBar.getPosition()
				);

				_contentBounds.setY(
					_verticalScrollMode == ScrollMode::disabled
					? bounds.getY()
					: bounds.getY() - _verticalScrollBar.getPosition()
				);

				_contentBounds.setWidth(
					_horizontalScrollMode == ScrollMode::disabled
					? bounds.getWidth()
					: measuredSize.getWidth()
				);

				_contentBounds.setHeight(
					_verticalScrollMode == ScrollMode::disabled
					? bounds.getHeight()
					: measuredSize.getHeight()
				);

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->render(renderer, _contentBounds);
				}

				const auto& processScrollBar = [&bounds, &renderer](ScrollBar& bar, bool& possible, ScrollMode mode, uint16_t size, uint16_t contentSize) {
					bar.setTotalSize(contentSize);
					bar.setViewportSize(size);
					possible = mode != ScrollMode::disabled && contentSize > size;

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
					bounds.getWidth(),
					_contentBounds.getWidth()
				);

				processScrollBar(
					_verticalScrollBar,
					_verticalScrollPossible,
					_verticalScrollMode,
					bounds.getHeight(),
					_contentBounds.getHeight()
				);
			}

			void onEvent(Event* event) override {
				Layout::onEvent(event);

				if (event->isHandled())
					return;

				const auto isTouchDown = event->getTypeID() == TouchDownEvent::typeID;
				const auto isTouchUp = event->getTypeID() == TouchUpEvent::typeID;
				const auto isTouchDrag = event->getTypeID() == TouchDragEvent::typeID;

				if (!(isTouchDown || isTouchUp || isTouchDrag))
					return;

				const auto& touchPosition = ((TouchEvent*) event)->getPosition();

				if (isTouchDown) {
					_lastTouchPosition = touchPosition;

					setCaptured(true);
				}
				else if (isTouchDrag) {
					if (_lastTouchPosition.getX() >= 0) {
						const auto touchDelta = touchPosition - _lastTouchPosition;
						_lastTouchPosition = touchPosition;

						if (_horizontalScrollPossible)
							scrollHorizontallyBy(-touchDelta.getX());

						if (_verticalScrollPossible)
							scrollVerticallyBy(-touchDelta.getY());
					}
				}
				else {
					_lastTouchPosition.setX(-1);

					setCaptured(false);
				}
			}

			void scrollHorizontallyBy(int32_t delta) {
				if (_horizontalScrollMode == ScrollMode::disabled)
					return;

				auto position = getHorizontalPosition();

				computeScrollPositionForDelta(
					position,
					delta,
					getBounds().getX2(),
					_contentBounds.getX2()
				);

				setHorizontalPosition(position);
			}

			void scrollVerticallyBy(int32_t delta) {
				if (_verticalScrollMode == ScrollMode::disabled)
					return;

				auto position = getVerticalPosition();

				computeScrollPositionForDelta(
					position,
					delta,
					getBounds().getY2(),
					_contentBounds.getY2()
				);

				setVerticalPosition(position);
			}

		private:
			ScrollBar _horizontalScrollBar = ScrollBar();
			ScrollBar _verticalScrollBar = ScrollBar();

			ScrollMode _horizontalScrollMode = ScrollMode::disabled;
			ScrollMode _verticalScrollMode = ScrollMode::computed;

			Bounds _contentBounds = Bounds();

			bool _horizontalScrollPossible = false;
			bool _verticalScrollPossible = false;

			Point _lastTouchPosition = Point(-1, -1);

			static void computeScrollPositionForDelta(uint16_t& position, int32_t delta, int32_t boundsPosition2, int32_t contentPosition2) {
				if (delta <= 0) {
					if (-delta > position) {
						position = 0;
					}
					else {
						position += delta;
					}
				}
				else {
					auto position2Delta = contentPosition2 - boundsPosition2;

					if (position2Delta > 0) {
						position += (delta > position2Delta ? position2Delta : delta);
					}
				}
			}
	};
}