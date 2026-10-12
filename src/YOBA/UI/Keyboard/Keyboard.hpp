#pragma once

#include <cstdint>
#include <vector>
#include <span>
#include <functional>

#include <YOBA/UI/Keyboard/Generic.hpp>
#include <YOBA/UI/Layout.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Shapes/RectangularShape.hpp>
#include <YOBA/Core/Text.hpp>

namespace YOBA {
	class Keyboard;
	class KeyboardLayout;
	class KeyboardButton;

	class Keyboard : public Layout, public FontElement {
		public:
			Keyboard();

			~Keyboard() override;

			KeyboardLayout* getLayout() const;
			void setLayout(KeyboardLayout* value);

			uint8_t getTextParsingOptions() const;
			void setLayoutFromTextParsingOptions(const uint8_t options = TextParsingOptions::none);

			uint8_t getCyclicLayoutsCount() const;
			uint8_t getCyclicLayoutIndex() const;
			void setLayoutFromCyclicIndex(uint8_t cyclicLayoutIndex);
			void setNextCyclicLayoutIndex();

			const Color* getBackgroundColor() const;

			void setBackgroundColor(const Color* value);

			const Color* getDefaultKeyBackgroundColor() const;
			void setDefaultKeyBackgroundColor(const Color* value);

			const Color* getDefaultKeyTextColor() const;
			void setDefaultKeyTextColor(const Color* value);

			const Color* getActionKeyBackgroundColor() const;
			void setActionKeyBackgroundColor(const Color* value);

			const Color* getDisabledKeyBackgroundColor() const;
			void setDisabledKeyBackgroundColor(const Color* value);

			const Color* getDisabledKeyTextColor() const;
			void setDisabledKeyTextColor(const Color* value);

			const Color* getActionKeyTextColor() const;
			void setActionKeyTextColor(const Color* actionButtonSecondaryColor);

			uint8_t getHorizontalKeyGap() const;
			void setHorizontalKeyGap(uint8_t value);

			uint8_t getVerticalKeyGap() const;
			void setVerticalKeyGap(uint8_t value);

			uint8_t getKeyCornerRadius() const;
			void setKeyCornerRadius(uint8_t value);

			uint16_t getKeyHeight() const;
			void setKeyHeight(uint16_t value);

			KeyboardCase getCase() const;
			void setCase(KeyboardCase value);

			std::span<std::function<KeyboardLayout*()>> getCyclicLayoutsGetter() const;
			void setCyclicLayoutsGetter(const std::span<std::function<KeyboardLayout*()>> value);

			const std::function<KeyboardLayout*()>& getCharactersLayoutGetter() const;
			void setCharactersLayoutGetter(const std::function<KeyboardLayout*()>& value);

			const std::function<KeyboardLayout*()>& getNumericLayoutGetter() const;
			void setNumericLayoutGetter(const std::function<KeyboardLayout*()>& value);

			void setOnLayoutChanged(const std::function<void(KeyboardLayout*, KeyboardLayout*)>& callback);

			uint16_t getContinuousTypingDelay() const;
			void setContinuousTypingDelay(uint16_t value);

			uint16_t getContinuousTypingInterval() const;
			void setContinuousTypingInterval(uint16_t value);

		private:
			const Color* _defaultKeyBackgroundColor = nullptr;
			const Color* _defaultKeyTextColor = nullptr;

			const Color* _actionKeyBackgroundColor = nullptr;
			const Color* _actionKeyTextColor = nullptr;

			const Color* _disabledKeyBackgroundColor = nullptr;
			const Color* _disabledKeyTextColor = nullptr;

			uint8_t _textParsingOptions = TextParsingOptions::none;
			bool _textParsingOptionsSet = false;

			KeyboardCase _case = KeyboardCase::lower;

			uint8_t _keyCornerRadius = 0;
			uint32_t _continuousTypingDelay = 500'000;
			uint32_t _continuousTypingInterval = 50'000;

			uint8_t _cyclicLayoutIndex = 0;
			KeyboardLayout* _layout = nullptr;
			std::span<std::function<KeyboardLayout*()>> _cyclicLayoutsGetter;
			std::function<KeyboardLayout*()> _charactersLayoutGetter = nullptr;
			std::function<KeyboardLayout*()> _numericLayoutGetter = nullptr;
			std::function<void(KeyboardLayout*, KeyboardLayout*)> _onLayoutChanged = nullptr;

			RectangularShape _backgroundPanel {};

			uint8_t _horizontalKeyGap = 1;
			uint8_t _verticalKeyGap = 1;
			uint16_t _keyHeight = 25;
	};
}