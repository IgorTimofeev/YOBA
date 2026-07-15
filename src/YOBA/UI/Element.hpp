#pragma once

#include <cstdint>
#include <limits>
#include <concepts>

#include <YOBA/System.hpp>
#include <YOBA/Core/Alignment.hpp>
#include <YOBA/Core/Margin.hpp>
#include <YOBA/Core/Rectangle.hpp>
#include <YOBA/Core/Events/Event.hpp>
#include <YOBA/Core/Size.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>

namespace YOBA {
	class Event;
	class Animation;
	class TargetAnimation;
	class Application;
	class Control;
	class Parent;
	class Layout;
	class Decorator;
	class Transform;

	class Element {
		friend Application;
		friend Parent;
		friend Layout;
		friend Decorator;
		friend Control;

		public:
			Element() = default;

			virtual ~Element();

			void measure(const Size& availableSize);
			void arrange(const Rectangle& parentBounds);
			void render(Renderer* renderer, const Rectangle& bounds);

			bool isPointerOver() const;
			virtual void invalidateRender();
			virtual void invalidateLayout();
			virtual void invalidate();

			Parent* getParent() const;

			template <typename T>
			requires std::derived_from<T, Parent>
			T* findParent() const;

			bool isVisible() const;
			void setVisible(bool value);

			bool isEnabled() const;
			void setEnabled(bool value);

			bool isVisibleForPointerEvents() const;
			void setVisibleForPointerEvents(bool value);

			bool isFocusable() const;
			void setFocusable(bool focusable);

			bool isFocused() const;
			void setFocused(bool state);

			bool isCaptured() const;
			void setCaptured(bool state);

			void scrollIntoView();
			bool isVisibleOnTarget() const;

			Alignment getHorizontalAlignment() const;
			void setHorizontalAlignment(Alignment value);

			Alignment getVerticalAlignment() const;
			void setVerticalAlignment(Alignment value);

			void setAlignment(Alignment horizontal, Alignment vertical);
			void setAlignment(Alignment uniformValue);

			bool getClipToBounds() const;
			void setClipToBounds(bool value);

			const Size& getSize() const;
			void setSize(const Size& value);

			/// @brief Shorthand for getSize().getWidth()
			uint16_t getWidth() const;
			/// @brief Shorthand for getSize().setWidth(value)
			void setWidth(uint16_t value);

			/// @brief Shorthand for getSize().getHeight()
			uint16_t getHeight() const;
			/// @brief Shorthand for getSize().setHeight(value)
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
			const Rectangle& getLayoutBounds() const;
			const Rectangle& getRenderingBounds() const;

			Transform* getRenderingTransform() const;
			void setRenderingTransform(Transform* transform);

			Transform* getLayoutTransform() const;
			void setLayoutTransform(Transform* transform);

		protected:
			virtual void onAddedToParent(Parent* parent);
			virtual void onRemovedFromParent(Parent* parent);

			virtual void onFocusChanged();
			virtual void onCaptureChanged();

			virtual void onTick();
			virtual Size onMeasure(const Size& availableSize);
			virtual void onArrange(const Rectangle& bounds);
			virtual void onRender(Renderer* renderer, const Rectangle& bounds);
			virtual void onBoundsChanged();

			virtual void onPointerOverChanged();

		private:
			bool _isVisible: 1 = true;
			bool _isEnabled: 1 = true;
			bool _clipToBounds: 1 = false;
			bool _focusable: 1 = true;
			bool _isPointerOver: 1 = false;
			bool _isVisibleForPointerEvents: 1 = true;

			Size _size { Size::computed, Size::computed };
			Size _minSize { 0, 0 };
			Size _maxSize { Size::computed, Size::computed };
			Alignment _horizontalAlignment = Alignment::stretch;
			Alignment _verticalAlignment = Alignment::stretch;
			Parent* _parent = nullptr;

			Transform* _layoutTransform = nullptr;
			Transform* _renderingTransform = nullptr;

			Size _measuredSize {};
			Rectangle _layoutBounds {};
			Rectangle _renderingBounds {};

			void addToParent(Parent* parent);
			void removeFromParent(Parent* parent);
			void setPointerOver(bool value);

			static void computeDefaultArrangeShit(
				Alignment alignment,
				int32_t boundsStart,
				uint16_t boundsSize,
				uint16_t size,
				uint16_t measuredSize,
				int32_t& newPosition,
				int32_t& newSize
			);

			virtual void handleEvent(Event* event, const Rectangle& parentBounds, const bool callHandlers) = 0;
	};
}
