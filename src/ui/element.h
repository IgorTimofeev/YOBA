#pragma once

#include <cstdint>
#include <limits>

#include "../action.h"
#include "../margin.h"
#include "../bounds.h"
#include "../size.h"
#include "../hardware/screen/buffers/screenBuffer.h"

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

	class Event;
	class Application;
	class Animation;

	class Element {
		public:
			Element() = default;

			virtual ~Element() = default;

			Size measure(ScreenBuffer* screenBuffer, const Size& availableSize);

			void arrange(const Bounds& bounds);

			virtual void tick();
			virtual void render(ScreenBuffer* screenBuffer);
			virtual void handleEvent(Event& event);

			virtual void invalidateRender();
			virtual void invalidateLayout();
			virtual void invalidate();

			virtual void startAnimation(Animation* animation);

			void addEventHandler(const std::function<void(Event&)>& handler);

			// -------------------------------- Getters & setters --------------------------------

			Application* getRoot();
			void setRoot(Application* value);

			Element* getParent();
			void setParent(Element* value);

			bool isVisible() const;
			void setVisible(bool value);

			bool isEnabled() const;
			void setEnabled(bool value);

			void setCaptured(const bool &value);
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

			const Size& getDesiredSize();

			const Bounds& getBounds();

		protected:
			virtual Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize);
			virtual void onArrange(const Bounds& bounds);
			virtual void onRender(ScreenBuffer* screenBuffer);
			virtual void onEvent(Event& event);

		private:
			bool _isVisible = true;
			bool _isEnabled = true;
			bool _clipToBounds = false;

			Size _size = Size(Size::Calculated, Size::Calculated);
			Alignment _horizontalAlignment = Alignment::Stretch;
			Alignment _verticalAlignment = Alignment::Stretch;
			Margin _margin = Margin::zero;
			Application* _root = nullptr;
			Element* _parent = nullptr;

			Bounds _bounds;
			Size _desiredSize = Size();

			Action<Event&> _eventHandlers {};

			void setDesiredSize(const Size& value);

			void setBounds(const Bounds& value);

			static void calculateMeasureShit(
				const Alignment &alignment,
				const uint16_t &size,
				const uint16_t &desiredSize,
				const int32_t &marginStart,
				const int32_t &marginEnd,
				int32_t &newSize
			);

			static void calculateArrangeShit(
				const Alignment& alignment,
				const int32_t& position,
				const uint16_t& size,
				const uint16_t& desiredSize,
				const int32_t& marginStart,
				const int32_t& marginEnd,
				const uint16_t& limit,
				int32_t& newPosition,
				int32_t& newSize
			);
	};
}