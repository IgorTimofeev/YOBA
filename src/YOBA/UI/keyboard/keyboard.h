#pragma once

#include <optional>

#include <YOBA/UI/keyboard/generic.h>
#include <YOBA/UI/keyboard/button.h>
#include <YOBA/UI/keyboard/buttonsLayout.h>
#include <YOBA/UI/keyboard/keyboardLayout.h>
#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/shapes/rectangle.h>
#include <YOBA/UI/layout.h>
#include <YOBA/UI/button.h>

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

			uint8_t getLayoutOptions() const;
			void setLayout(const uint8_t options = KeyboardLayoutOptions::text);

			uint8_t getCyclicLayoutsCount() const;
			uint8_t getCyclicLayoutIndex() const;
			void setCyclicLayoutIndex(uint8_t value);
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

			uint8_t getHorizontalKeySpacing() const;
			void setHorizontalKeySpacing(uint8_t horizontalKeySpacing);

			uint8_t getVerticalKeySpacing() const;
			void setVerticalKeySpacing(uint8_t value);

			uint8_t getKeyCornerRadius() const;
			void setKeyCornerRadius(uint8_t value);

			uint16_t getKeyHeight() const;
			void setKeyHeight(uint16_t value);

			KeyboardCase getCase() const;
			void setCase(KeyboardCase value);

			std::vector<std::function<KeyboardLayout*()>>& getCyclicLayoutBuilders();
			void setCyclicLayoutBuilders(const std::vector<std::function<KeyboardLayout*()>>& builder);

			const std::optional<std::function<KeyboardLayout*()>>& getCharactersLayoutBuilder() const;
			void setCharactersLayoutBuilder(const std::optional<std::function<KeyboardLayout*()>>& builder);

			const std::optional<std::function<KeyboardLayout*()>>& getNumericLayoutBuilder() const;
			void setNumericLayoutBuilder(const std::optional<std::function<KeyboardLayout*()>>& builder);

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

			uint8_t _layoutOptions = 0;
			KeyboardCase _case = KeyboardCase::lower;

			uint8_t _keyCornerRadius = 0;
			uint32_t _continuousTypingDelay = 500'000;
			uint32_t _continuousTypingInterval = 50'000;

			uint8_t _cyclicLayoutIndex = 0;
			KeyboardLayout* _layout = nullptr;
			std::vector<std::function<KeyboardLayout*()>> _cyclicLayoutBuilders;
			std::optional<std::function<KeyboardLayout*()>> _charactersLayoutBuilder = std::nullopt;
			std::optional<std::function<KeyboardLayout*()>> _numericLayoutBuilder = std::nullopt;

			Rectangle _backgroundPanel = Rectangle();
			KeyboardButtonsLayout _buttonsLayout;

			uint8_t _horizontalKeySpacing = 1;
			uint8_t _verticalKeySpacing = 1;
			uint16_t _keyHeight = 25;

			void deleteLayoutAndUIElements();
	};
}