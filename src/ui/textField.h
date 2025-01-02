#pragma once

#include "element.h"
#include "cornerRadiusElement.h"
#include "primaryColorElement.h"
#include "secondaryColorElement.h"
#include "textElement.h"
#include "fontElement.h"
#include "borderElement.h"

#include "../callback.h"
#include "../event.h"
#include "../hardware/screen/buffers/screenBuffer.h"

namespace yoba {
	class TextField :
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

			void setFocused(bool value) override;

			void showKeyboard();
			const Color* getKeyboardBackgroundColor() const;
			void setKeyboardBackgroundColor(const Color* keyboardBackgroundColor);

			const Color* getKeyboardDefaultButtonPrimaryColor() const;
			void setKeyboardDefaultButtonPrimaryColor(const Color* keyboardTextButtonPrimaryColor);

			const Color* getKeyboardDefaultButtonSecondaryColor() const;
			void setKeyboardDefaultButtonSecondaryColor(const Color* keyboardTextButtonSecondaryColor);

			const Color* getKeyboardActionButtonPrimaryColor() const;
			void setKeyboardActionButtonPrimaryColor(const Color* keyboardActionButtonPrimaryColor);

			const Color* getKeyboardActionButtonSecondaryColor() const;
			void setKeyboardActionButtonSecondaryColor(const Color* keyboardActionButtonSecondaryColor);

			const Font* getKeyboardFont() const;
			void setKeyboardFont(const Font* keyboardFont);

			const Size& getCursorSize() const;
			void setCursorSize(const Size& cursorSize);

			const Color* getCursorColor() const;
			void setCursorColor(const Color* cursorColor);

			uint32_t getCursorBlinkInterval() const;
			void setCursorBlinkInterval(uint32_t cursorBlinkInterval);

			size_t getCursorPosition() const;
			void setCursorPosition(size_t cursorPosition);

			void setCursorToStart();

			void setCursorToEnd();

			void insert(const std::wstring_view& value);
			void backspace();

		private:
			uint32_t _cursorBlinkTime = 0;
			uint32_t _cursorBlinkInterval = 500;
			bool _cursorBlinkState = false;
			size_t _cursorPosition = 0;
			Size _cursorSize = Size(2, 14);

			const Font* _keyboardFont = nullptr;
			const Color* _cursorColor = nullptr;
			const Color* _keyboardBackgroundColor = nullptr;
			const Color* _keyboardDefaultButtonPrimaryColor = nullptr;
			const Color* _keyboardDefaultButtonSecondaryColor = nullptr;
			const Color* _keyboardActionButtonPrimaryColor = nullptr;
			const Color* _keyboardActionButtonSecondaryColor = nullptr;

			void setCursorBlinkStateAndTime(bool value);
	};
}
