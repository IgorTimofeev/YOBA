#pragma once

#include <cstdint>
#include "element.h"
#include "traits/focusableElement.h"
#include "traits/cornerRadiusElement.h"
#include "traits/primaryColorElement.h"
#include "traits/secondaryColorElement.h"
#include "traits/textElement.h"
#include "traits/fontElement.h"
#include "traits/borderElement.h"
#include "traits/fontScaleElement.h"
#include "keyboard.h"

#include "main/callback.h"
#include "main/event.h"
#include "rendering/renderers/renderer.h"

namespace yoba::ui {
	class TextField :
		public FocusableElement,
		public PrimaryColorElement,
		public SecondaryColorElement,
		public CornerRadiusElement,
		public BorderElement,
		public TextElement,
		public FontElement,
		public FontScaleElement
	{
		public:
			void onTick() override;

			void onRender(Renderer* renderer) override;

			void onEvent(Event& targetX) override;

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
			int32_t _lastTouchX = -1;
			uint16_t _textMargin = 10;

			uint32_t _cursorBlinkTime = 0;
			uint32_t _cursorBlinkInterval = 500;
			bool _cursorBlinkState = false;
			size_t _cursorPosition = 0;
			Size _cursorSize = Size(2, 14);

			uint16_t _scrollPosition = 0;
			uint32_t _continuousScrollTime = 0;
			uint32_t _continuousScrollInterval = 50;

			const Color* _focusedBorderColor = nullptr;
			const Color* _focusedPrimaryColor = nullptr;
			const Color* _focusedSecondaryColor = nullptr;
			const Color* _cursorColor = nullptr;

			std::optional<std::function<void(Keyboard*)>> _keyboardConfigurator = std::nullopt;

			void setCursorBlinkStateAndTime(bool value);
			void showKeyboard();

			void applyContinuousScroll();
	};
}
