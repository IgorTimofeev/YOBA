#pragma once

#include <optional>
#include <functional>

#include <YOBA/Rendering/Renderer.hpp>
#include <YOBA/Core/Key.hpp>

#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/TextElement.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Traits/FontScaleElement.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Keyboard/Generic.hpp>

namespace YOBA {
	class TextField :
		public Control,
		public CornerRadiusElement,
		public TextElement,
		public FontElement,
		public FontScaleElement
	{
		public:
			uint8_t getKeyboardLayoutOptions() const;
			void setKeyboardLayoutOptions(const uint8_t keyboardLayoutOptions);

			char getMask() const;
			void setMask(char mask);

			std::string_view getPlaceholder() const;
			void setPlaceholder(const std::string_view& value);

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

			void insert(std::string_view value);
			void backspace();

			void setOnInput(const std::function<void(Key, std::optional<std::string_view>)>& callback);

		protected:
			void onTick() override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;
			void onEvent(Event* event) override;
			void onFocusChanged() override;

			virtual void onInput(Key key, std::optional<std::string_view> text);

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

			std::string _placeholder = std::string();
			char _mask = 0;
			uint8_t _keyboardLayoutOptions = KeyboardLayoutOptions::text;

			std::function<void(Key, std::optional<std::string_view>)> _onInput = nullptr;

			void setCursorBlinkStateAndTime(bool value);
			void applyContinuousScroll();
	};
}
