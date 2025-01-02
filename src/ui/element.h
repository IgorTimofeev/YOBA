#pragma once

#include <cstdint>
#include <limits>

#include "../callback.h"
#include "../margin.h"
#include "../bounds.h"
#include "../size.h"
#include "hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	enum class Alignment: uint8_t {
		Start,
		Center,
		End,
		Stretch
	};

	enum class Orientation: uint8_t {
		Horizontal,
		Vertical
	};

	class InputEvent;
	class Application;
	class Animation;
	class Container;

	class Element {
		friend Container;

		public:
			Element() = default;

			virtual ~Element() = default;

			void measure(ScreenBuffer* screenBuffer, const Size& availableSize);
			void arrange(const Bounds& bounds);

			virtual void render(ScreenBuffer* screenBuffer);
			virtual void handleEvent(InputEvent& event);

			virtual void invalidateRender();
			virtual void invalidateLayout();
			virtual void invalidate();

			virtual void startAnimation(Animation* animation);

			Callback<InputEvent&>& getEventHandlers();

			Application* getRoot();
			Element* getParent();

			bool isVisible() const;
			void setVisible(bool value);

			bool isEnabled() const;
			void setEnabled(bool value);

			void setCaptured(bool value);
			bool isCaptured();

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
			virtual void setRoot(Application* value);

			virtual void onAddedToParent(Container* parent);
			virtual void onRemovedFromParent(Container* parent);

			virtual Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize);
			virtual void onArrange(const Bounds& bounds);
			virtual void onRender(ScreenBuffer* screenBuffer);
			virtual void onEvent(InputEvent& event);

		private:
			bool _isVisible = true;
			bool _isEnabled = true;
			bool _clipToBounds = false;

			Size _size = Size(Size::Auto, Size::Auto);
			Alignment _horizontalAlignment = Alignment::Stretch;
			Alignment _verticalAlignment = Alignment::Stretch;
			Margin _margin = Margin::zero;
			Application* _root = nullptr;
			Element* _parent = nullptr;

			Bounds _bounds;
			Size _measuredSize = Size();

			Callback<InputEvent&> _eventHandlers {};

			void setMeasuredSize(const Size& value);

			void setBounds(const Bounds& value);

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