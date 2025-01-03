#pragma once

#include <cstdint>
#include "element.h"
#include "focusableElement.h"
#include "cornerRadiusElement.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "textElement.h"
#include "fontElement.h"
#include "borderElement.h"
#include "keyboard.h"

#include "../callback.h"
#include "../event.h"
#include "../hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	class TextField :
		public FocusableElement,
		public PrimaryColorElement,
		public SecondaryColorElement,
		public CornerRadiusElement,
		public BorderElement,
		public TextElement
	{
		public:
			void tick() override;

			void onRender(ScreenBuffer* screenBuffer) override;

			void onEvent(InputEvent &event) override;

			void onFocusChanged() override;

			const uint16_t& getTextMargin() const;
			void setTextMargin(const uint16_t& textMargin);

			const Size& getCursorSize() const;
			void setCursorSize(const Size& cursorSize);

			const Color* getCursorColor() const;
			void setCursorColor(const Color* cursorColor);

			const Color* getFocusedBorderColor() const;

			void setFocusedBorderColor(const Color* focusedBorderColor);

			const Color* getFocusedPrimaryColor() const;

			void setFocusedPrimaryColor(const Color* focusedPrimaryColor);

			const Color* getFocusedSecondaryColor() const;

			void setFocusedSecondaryColor(const Color* focusedSecondaryColor);

			uint32_t getCursorBlinkInterval() const;
			void setCursorBlinkInterval(uint32_t cursorBlinkInterval);

			size_t getCursorPosition() const;
			void setCursorPosition(size_t cursorPosition);

			void setCursorToStart();
			void setCursorToEnd();

			void insert(const std::wstring_view& value);
			void backspace();

			const std::optional<std::function<void(Keyboard*)>>& getKeyboardConfigurator() const;
			void setKeyboardConfigurator(const std::optional<std::function<void(Keyboard*)>>& keyboardConfigurator);

		private:
			uint32_t _cursorBlinkTime = 0;
			uint32_t _cursorBlinkInterval = 500;
			bool _cursorBlinkState = false;
			size_t _cursorPosition = 0;
			Size _cursorSize = Size(2, 14);
			uint16_t _textMargin = 10;
			int32_t _scrollValue = 0;

			const Color* _focusedBorderColor = nullptr;
			const Color* _focusedPrimaryColor = nullptr;
			const Color* _focusedSecondaryColor = nullptr;
			const Color* _cursorColor = nullptr;

			std::optional<std::function<void(Keyboard*)>> _keyboardConfigurator = std::nullopt;

			void setCursorBlinkStateAndTime(bool value);
			void showKeyboard();
	};
}
