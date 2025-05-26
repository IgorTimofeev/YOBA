#pragma once

#include <cstdint>
#include <limits>

#include <YOBA/system.h>
#include <YOBA/main/callback.h>
#include <YOBA/main/margin.h>
#include <YOBA/main/bounds.h>
#include <YOBA/main/event.h>
#include <YOBA/main/size.h>
#include <YOBA/main/rendering/renderer.h>

namespace YOBA {
	class Event;
}

namespace YOBA {
	class Animation;

	enum class Alignment: uint8_t {
		start,
		center,
		end,
		stretch
	};

	enum class Orientation: uint8_t {
		horizontal,
		vertical
	};

	class Application;
	class Control;
	class Layout;

	class Element {
		friend Application;
		friend Layout;
		friend Control;

		public:
			Element() = default;

			virtual ~Element();

			void measure(const Size& availableSize);
			void render(Renderer* renderer, const Bounds& bounds);

			virtual void pushEvent(Event* event) = 0;

			bool isTouchOver() const;
			virtual void invalidateRender();
			virtual void invalidateMeasure();
			virtual void invalidate();

			virtual void startAnimation(Animation* animation);

			Layout* getParent() const;

			bool isVisible() const;
			void setVisible(bool value);

			bool isEnabled() const;
			void setEnabled(bool value);

			bool isFocusable() const;
			void setFocusable(bool focusable);

			bool isFocused() const;
			void setFocused(bool state);

			bool isCaptured() const;
			void setCaptured(bool state);

			Alignment getHorizontalAlignment() const;
			void setHorizontalAlignment(Alignment value);

			Alignment getVerticalAlignment() const;
			void setVerticalAlignment(Alignment value);

			void setAlignment(Alignment horizontal, Alignment vertical);
			void setAlignment(Alignment uniformValue);

			bool getClipToBounds() const;
			void setClipToBounds(bool value);

			const Margin& getMargin() const;
			void setMargin(const Margin& value);

			const Size& getSize() const;
			void setSize(const Size& value);
			void setWidth(uint16_t value);
			void setHeight(uint16_t value);

			const Size& getMaxSize() const;
			void setMaxSize(const Size& value);
			void setMaxWidth(uint16_t value);
			void setMaxHeight(uint16_t value);

			const Size& getMinSize() const;
			void setMinSize(const Size& value);
			void setMinWidth(uint16_t value);
			void setMinHeight(uint16_t value);

			const Size& getMeasuredSize() const;
			const Bounds& getBounds() const;

		protected:
			virtual void onAddedToParent(Layout* parent);
			virtual void onRemovedFromParent(Layout* parent);

			virtual void onFocusChanged();
			virtual void onCaptureChanged();

			virtual void onTick();
			virtual Size onMeasure(const Size& availableSize);
			virtual void onRender(Renderer* renderer, const Bounds& bounds);
			virtual void onBoundsChanged();

			virtual void onTouchOverChanged();

		private:
			bool _isVisible = true;
			bool _isEnabled = true;
			bool _clipToBounds = false;
			bool _focusable = true;

			Size _size = Size(Size::computed, Size::computed);
			Size _minSize = Size(0, 0);
			Size _maxSize = Size(0xFFFF, 0xFFFF);
			Alignment _horizontalAlignment = Alignment::stretch;
			Alignment _verticalAlignment = Alignment::stretch;
			Margin _margin = Margin::zero;
			Layout* _parent = nullptr;

			Bounds _bounds {};
			Size _measuredSize {};

			void setTouchOver(bool state);

			template<typename TClass>
			void callEventSpecificFunctions(
				TClass* instance,

				void(TClass::*onTouchDown)(TouchDownEvent* event),
				void(TClass::*onTouchDrag)(TouchDragEvent* event),
				void(TClass::*onTouchUp)(TouchUpEvent* event),

				void(TClass::*onPinchDown)(PinchDownEvent* event),
				void(TClass::*onPinchDrag)(PinchDragEvent* event),
				void(TClass::*onPinchUp)(PinchUpEvent* event),

				Event* event
			);

			static uint16_t computeMeasureShit(
				uint16_t size,
				uint16_t desiredSize,
				int32_t marginStart,
				int32_t marginEnd,
				uint16_t min,
				uint16_t max
			);

			static void computeBoundsShit(
				Alignment alignment,
				int32_t position,
				uint16_t size,
				uint16_t desiredSize,
				int32_t marginStart,
				int32_t marginEnd,
				uint16_t bounds,

				int32_t& newPosition,
				int32_t& newSize
			);
	};

	template<typename TClass>
	void Element::callEventSpecificFunctions(
		TClass* instance,

		void(TClass::*onTouchDown)(TouchDownEvent* event),
		void(TClass::*onTouchDrag)(TouchDragEvent* event),
		void(TClass::*onTouchUp)(TouchUpEvent* event),

		void(TClass::*onPinchDown)(PinchDownEvent* event),
		void(TClass::*onPinchDrag)(PinchDragEvent* event),
		void(TClass::*onPinchUp)(PinchUpEvent* event),

		Event* event
	) {
		if (TouchEvent::isTouch(event)) {
			if (isCaptured() || getBounds().intersects(reinterpret_cast<TouchEvent*>(event)->getPosition())) {
				if (event->getTypeID() == TouchDownEvent::typeID) {
					(instance->*onTouchDown)(reinterpret_cast<TouchDownEvent*>(event));
				}
				else if (event->getTypeID() == TouchDragEvent::typeID) {
					(instance->*onTouchDrag)(reinterpret_cast<TouchDragEvent*>(event));
				}
				else if (event->getTypeID() == TouchUpEvent::typeID) {
					(instance->*onTouchUp)(reinterpret_cast<TouchUpEvent*>(event));
				}

				if (event->isHandled()) {
					setTouchOver(true);
				}
			}
		}
		else if (PinchEvent::isPinch(event)) {
			const auto pinchEvent = reinterpret_cast<PinchEvent*>(event);
			const auto& bounds = getBounds();

			if (isCaptured() || (bounds.intersects(pinchEvent->getPosition1()) && bounds.intersects(pinchEvent->getPosition2()))) {
				if (event->getTypeID() == PinchDownEvent::typeID) {
					(instance->*onPinchDown)(reinterpret_cast<PinchDownEvent*>(event));
				}
				else if (event->getTypeID() == PinchDragEvent::typeID) {
					(instance->*onPinchDrag)(reinterpret_cast<PinchDragEvent*>(event));
				}
				else if (event->getTypeID() == PinchUpEvent::typeID) {
					(instance->*onPinchUp)(reinterpret_cast<PinchUpEvent*>(event));
				}

				if (event->isHandled()) {
					setTouchOver(true);
				}
			}
		}
	}
}
