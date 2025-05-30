#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>
#include <YOBA/UI/traits/textElement.h>
#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/traits/fontScaleElement.h>
#include "keyboard.h"

#include <YOBA/main/callback.h>
#include <YOBA/main/events/pointerEvent.h>
#include <YOBA/main/events/pinchEvent.h>
#include <YOBA/main/rendering/renderer.h>

namespace YOBA {
	class TextField :
		public Control,
		public CornerRadiusElement,
		public TextElement,
		public FontElement,
		public FontScaleElement
	{
		public:
			~TextField() override;

			Callback<Key, std::wstring_view> input {};
			Callback<> textChanged {};

			wchar_t getMask() const;
			void setMask(wchar_t mask);

			std::wstring_view getPlaceholder() const;
			void setPlaceholder(const std::wstring_view& value);

			const uint16_t& getTextMargin() const;
			void setTextMargin(const uint16_t& textMargin);

			const Size& getCursorSize() const;
			void setCursorSize(const Size& cursorSize);

			const Color* getCursorColor() const;
			void setCursorColor(const Color* cursorColor);

			const Color* getDefaultBorderColor() const;
			void setDefaultBorderColor(const Color* value);

			const Color* getDefaultBackgroundColor() const;
			void setDefaultBackgroundColor(const Color* value);

			const Color* getDefaultTextColor() const;
			void setDefaultTextColor(const Color* value);

			const Color* getFocusedBorderColor() const;
			void setFocusedBorderColor(const Color* value);

			const Color* getFocusedBackgroundColor() const;
			void setFocusedBackgroundColor(const Color* value);

			const Color* getFocusedTextColor() const;
			void setFocusedTextColor(const Color* value);

			const Color* getPlaceholderColor() const;
			void setPlaceholderColor(const Color* value);

			uint32_t getCursorBlinkInterval() const;
			void setCursorBlinkInterval(uint32_t value);

			size_t getCursorPosition() const;
			void setCursorPosition(size_t value);

			void setCursorToStart();
			void setCursorToEnd();

			void insert(std::wstring_view value);
			void backspace();

		protected:
			void onTick() override;
			void onRender(Renderer* renderer, const Bounds& bounds) override;
			void onEvent(Event* event) override;
			void onFocusChanged() override;
			void onTextChanged() override;

			virtual void onInput(Key keyCode, std::wstring_view text);

		private:
			int32_t _lastTouchX = -1;
			uint16_t _textMargin = 10;

			uint64_t _cursorBlinkTime = 0;
			uint32_t _cursorBlinkInterval = 500'000;
			bool _cursorBlinkState = false;
			size_t _cursorPosition = 0;
			Size _cursorSize = Size(2, 14);

			uint16_t _scrollPosition = 0;
			uint64_t _continuousScrollTime = 0;
			uint32_t _continuousScrollInterval = 50'000;

			const Color* _defaultBackgroundColor = nullptr;
			const Color* _defaultTextColor = nullptr;
			const Color* _defaultBorderColor = nullptr;

			const Color* _focusedBorderColor = nullptr;
			const Color* _focusedBackgroundColor = nullptr;
			const Color* _focusedTextColor = nullptr;

			const Color* _placeholderColor = nullptr;
			const Color* _cursorColor = nullptr;

			std::wstring _placeholder = std::wstring();
			wchar_t _mask = 0;

			void setCursorBlinkStateAndTime(bool value);
			void applyContinuousScroll();
	};
}
