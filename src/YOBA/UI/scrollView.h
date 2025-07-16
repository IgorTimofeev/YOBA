#pragma once

#include "scrollBar.h"

#include <YOBA/UI/layout.h>

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

				horizontalScrollBar.setOrientation(Orientation::horizontal);
				horizontalScrollBar.setAlignment(Alignment::stretch, Alignment::end);

				verticalScrollBar.setOrientation(Orientation::vertical);
				verticalScrollBar.setAlignment(Alignment::end, Alignment::stretch);

				setScrollBarSize(3);
				setScrollBarOffset(3);
			}

			ScrollMode getHorizontalScrollMode() const {
				return horizontalScrollMode;
			}

			void setHorizontalScrollMode(const ScrollMode value) {
				horizontalScrollMode = value;

				invalidate();
			}

			ScrollMode getVerticalScrollMode() const {
				return verticalScrollMode;
			}

			void setVerticalScrollMode(const ScrollMode value) {
				verticalScrollMode = value;

				invalidate();
			}

			uint16_t getHorizontalPosition() const {
				return horizontalScrollBar.getPosition();
			}

			void setHorizontalPosition(const uint16_t value) {
				if (!horizontalScrollPossible)
					return;

				horizontalScrollBar.setPosition(value);

				invalidate();
			}

			uint16_t getVerticalPosition() const {

				return verticalScrollBar.getPosition();
			}

			void setVerticalPosition(const uint16_t value) {
				if (!verticalScrollPossible)
					return;

				verticalScrollBar.setPosition(value);

				invalidate();
			}

			void setScrollBarOffset(const uint16_t value) {
				horizontalScrollBar.setMargin(Margin(value, 0, value, value));
				verticalScrollBar.setMargin(Margin(0, value, value, value));
			}

			void setScrollBarSize(const uint16_t value) {
				horizontalScrollBar.setSize(Size(Size::computed, value));
				verticalScrollBar.setSize(Size(value, Size::computed));
			}

			void setScrollBarCornerRadius(const uint16_t value) {
				horizontalScrollBar.setCornerRadius(value);
				verticalScrollBar.setCornerRadius(value);
			}

			void setScrollBarThumbColor(const Color* value) {
				horizontalScrollBar.setThumbColor(value);
				verticalScrollBar.setThumbColor(value);
			}

			const Bounds& getContentBounds() const {
				return contentBounds;
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

		protected:
			Size onMeasure(const Size& availableSize) override {
				const auto& contentSize = Size(
					horizontalScrollMode == ScrollMode::disabled
					? availableSize.getWidth()
					: Size::unlimited,

					verticalScrollMode == ScrollMode::disabled
					? availableSize.getHeight()
					: Size::unlimited
				);

				contentMeasuredSize.setWidth(0);
				contentMeasuredSize.setHeight(0);

				for (const auto element : *this) {
					if (!element->isVisible())
						continue;

					element->measure(contentSize);

					const auto& elementSize = element->getMeasuredSize();

					if (elementSize.getWidth() > contentMeasuredSize.getWidth())
						contentMeasuredSize.setWidth(elementSize.getWidth());

					if (elementSize.getHeight() > contentMeasuredSize.getHeight())
						contentMeasuredSize.setHeight(elementSize.getHeight());
				}

				horizontalScrollBar.measure(availableSize);
				verticalScrollBar.measure(availableSize);

				return contentMeasuredSize;
			}

			void onRender(Renderer* renderer, const Bounds& bounds) override {
				if (horizontalScrollMode == ScrollMode::disabled) {
					if (horizontalScrollBar.getPosition() > 0)
						horizontalScrollBar.setPosition(0);

					contentBounds.setX(bounds.getX());
					contentBounds.setWidth(bounds.getWidth());
				}
				else {
					if (horizontalScrollBar.getPosition() > 0) {
						if (contentMeasuredSize.getWidth() > bounds.getWidth()) {
							if (bounds.getX() + contentMeasuredSize.getWidth() - 1 - horizontalScrollBar.getPosition() < bounds.getX2())
								horizontalScrollBar.setPosition(contentMeasuredSize.getWidth() - bounds.getWidth());
						}
						else {
							horizontalScrollBar.setPosition(0);
						}
					}

					contentBounds.setX(bounds.getX() - horizontalScrollBar.getPosition());
					contentBounds.setWidth(contentMeasuredSize.getWidth());
				}

				if (verticalScrollMode == ScrollMode::disabled) {
					if (verticalScrollBar.getPosition() > 0)
						verticalScrollBar.setPosition(0);

					contentBounds.setY(bounds.getY());
					contentBounds.setHeight(bounds.getHeight());
				}
				else {
					if (verticalScrollBar.getPosition() > 0) {
						// If content height > scroll view height
						if (contentMeasuredSize.getHeight() > bounds.getHeight()) {
							// If content Y2 with position < scroll view Y2
							if (bounds.getY() + contentMeasuredSize.getHeight() - 1 - verticalScrollBar.getPosition() < bounds.getY2())
								verticalScrollBar.setPosition(contentMeasuredSize.getHeight() - bounds.getHeight());
						}
						else {
							verticalScrollBar.setPosition(0);
						}
					}

					contentBounds.setY(bounds.getY() - verticalScrollBar.getPosition());
					contentBounds.setHeight(contentMeasuredSize.getHeight());
				}

				for (const auto element : *this)
					if (element->isVisible())
						element->render(renderer, contentBounds);

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
					horizontalScrollBar,
					horizontalScrollPossible,
					horizontalScrollMode,
					contentBounds.getWidth(),
					bounds.getWidth()
				);

				processScrollBar(
					verticalScrollBar,
					verticalScrollPossible,
					verticalScrollMode,
					contentBounds.getHeight(),
					bounds.getHeight()
				);

				// ESP_LOGI("SCROLL", "contentSize: %f, viewportSize: %f", (float) contentBounds.getHeight(), (float) bounds.getHeight());
			}

			void onCaptureChanged() override {
				Layout::onCaptureChanged();

				if (!isCaptured())
					lastTouchPosition.setX(-1);
			}

			void onEventBeforeChildren(Event* event) override {
				if (event->getTypeID() == PointerDownEvent::typeID) {
					lastTouchPosition = reinterpret_cast<PointerDownEvent*>(event)->getPosition();
				}
				else if (event->getTypeID() == PointerDragEvent::typeID) {
					if (lastTouchPosition.getX() >= 0) {
						const auto position = reinterpret_cast<PointerDragEvent*>(event)->getPosition();
						const auto pointerDelta = position - lastTouchPosition;

						if (isCaptured()) {
							lastTouchPosition = position;

							scrollHorizontallyBy(-pointerDelta.getX());
							scrollVerticallyBy(-pointerDelta.getY());
						}
						else {
							if (pointerDelta.getLength() >= 3) {
								setCaptured(true);

								lastTouchPosition = position;
							}
						}
					}
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
			ScrollBar horizontalScrollBar {};
			ScrollBar verticalScrollBar {};

			ScrollMode horizontalScrollMode = ScrollMode::disabled;
			ScrollMode verticalScrollMode = ScrollMode::computed;

			Size contentMeasuredSize {};
			Bounds contentBounds {};

			bool horizontalScrollPossible = false;
			bool verticalScrollPossible = false;

			Point lastTouchPosition { -1, -1 };
	};
}