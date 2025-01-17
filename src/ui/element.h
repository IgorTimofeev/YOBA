#pragma once

#include <cstdint>
#include <limits>

#include "callback.h"
#include "margin.h"
#include "bounds.h"
#include "size.h"
#include "rendering/renderer.h"

namespace yoba {
	class Animation;
	class Event;
}

namespace yoba::ui {
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
	class Layout;

	class Element {
		friend Application;
		friend Layout;

		public:
			Element() = default;

			virtual ~Element();

			void measure(Renderer* renderer, const Size& availableSize);
			void arrange(const Bounds& bounds);

			virtual void render(Renderer* renderer);
			virtual void handleEvent(Event& event);

			virtual void invalidateRender();
			virtual void invalidateLayout();
			virtual void invalidate();

			virtual void startAnimation(Animation* animation);

			Application* getApplication();
			Element* getParent();

			bool isVisible() const;
			void setVisible(bool value);

			bool isEnabled() const;
			void setEnabled(bool value);

			bool isCaptured();
			void setCaptured(bool value);

			Alignment getHorizontalAlignment() const;
			void setHorizontalAlignment(Alignment value);

			Alignment getVerticalAlignment() const;
			void setVerticalAlignment(Alignment value);

			void setAlignment(Alignment horizontal, Alignment vertical);
			void setAlignment(Alignment uniformValue);

			bool getClipToBounds() const;
			void setClipToBounds(bool value);

			const Margin& getMargin();
			void setMargin(const Margin& value);

			const Size& getSize();
			void setSize(const Size& value);

			void setWidth(uint16_t value);
			void setHeight(uint16_t value);

			const Size& getMeasuredSize();
			const Bounds& getBounds();

		protected:
			virtual void setApplication(Application* value);

			virtual void onAddedToParent(Layout* parent);
			virtual void onRemovedFromParent(Layout* parent);

			virtual void onFocusChanged();
			virtual void onCaptureChanged();

			virtual void onTick();
			virtual Size onMeasure(Renderer* renderer, const Size& availableSize);
			virtual void onArrange(const Bounds& bounds);
			virtual void onBoundsChanged();
			virtual void onRender(Renderer* renderer);
			virtual void onEvent(Event& event);

		private:
			bool _isVisible = true;
			bool _isEnabled = true;
			bool _clipToBounds = false;

			Size _size = Size(Size::computed, Size::computed);
			Alignment _horizontalAlignment = Alignment::stretch;
			Alignment _verticalAlignment = Alignment::stretch;
			Margin _margin = Margin::zero;
			Application* _application = nullptr;
			Layout* _parent = nullptr;

			Bounds _bounds;
			Size _measuredSize = Size();

			static void computeMeasureShit(
				const uint16_t &size,
				const uint16_t &desiredSize,
				const int32_t &marginStart,
				const int32_t &marginEnd,
				int32_t &newSize
			);

			static void computeArrangeShit(
				const Alignment& alignment,
				const int32_t& position,
				const uint16_t& size,
				const uint16_t& measuredSize,
				const int32_t& marginStart,
				const int32_t& marginEnd,
				const uint16_t& limit,
				int32_t& newPosition,
				int32_t& newSize
			);
	};
}