#pragma once

#include <optional>
#include "layout.h"
#include "fontElement.h"
#include "button.h"
#include "rectangle.h"
#include "stackLayout.h"

namespace yoba {
	enum class KeyCode : uint16_t {
		None = 0,	
		Cancel = 3,
		Backspace = 8,
		Tab = 9,
		Enter = 10,
		Clear = 12,
		Shift = 16,
		Ctrl = 17,
		Alt = 18,
		Pause = 19,
		CapsLock = 20,
		Kana = 21,
		Final = 24,
		Kanji = 25,
		Escape = 27,
		Convert = 28,
		NoConvert = 29,
		Accept = 30,
		ModeChange = 31,
		Space = 32,
		PageUp = 33,
		PageDown = 34,
		End = 35,
		Home = 36,
		Left = 37,
		Up = 38,
		Right = 39,
		Down = 40,
		Comma = 44,
		Minus = 45,
		Period = 46,
		Slash = 47,
		Number0 = 48,
		Number1 = 49,
		Number2 = 50,
		Number3 = 51,
		Number4 = 52,
		Number5 = 53,
		Number6 = 54,
		Number7 = 55,
		Number8 = 56,
		Number9 = 57,
		Semicolon = 59,
		Equals = 61,
		A = 65,
		B = 66,
		C = 67,
		D = 68,
		E = 69,
		F = 70,
		G = 71,
		H = 72,
		I = 73,
		J = 74,
		K = 75,
		L = 76,
		M = 77,
		N = 78,
		O = 79,
		P = 80,
		Q = 81,
		R = 82,
		S = 83,
		T = 84,
		U = 85,
		V = 86,
		W = 87,
		X = 88,
		Y = 89,
		Z = 90,
		OpenBracket = 91,
		BackSlash = 92,
		CloseBracket = 93,
		NumPad0 = 96,
		NumPad1 = 97,
		NumPad2 = 98,
		NumPad3 = 99,
		NumPad4 = 100,
		NumPad5 = 101,
		NumPad6 = 102,
		NumPad7 = 103,
		NumPad8 = 104,
		NumPad9 = 105,
		NumPadAsterisk = 106,
		NumPadPlus = 107,
		NumPadComma = 108,
		NumPadMinusf = 109,
		NumPadPeriod = 110,
		NumPadSlash = 111,
		F1 = 112,
		F2 = 113,
		F3 = 114,
		F4 = 115,
		F5 = 116,
		F6 = 117,
		F7 = 118,
		F8 = 119,
		F9 = 120,
		F10 = 121,
		F11 = 122,
		F12 = 123,
		Delete = 127,
		DeadGrave = 128,
		DeadAcute = 129,
		DeadCircumflex = 130,
		DeadTilde = 131,
		DeadMacron = 132,
		DeadBreve = 133,
		DeadAboveDot = 134,
		DeadDiaeresis = 135,
		DeadAboveRing = 136,
		DeadDoubleAcute = 137,
		DeadCaron = 138,
		DeadCedilla = 139,
		DeadOgonek = 140,
		DeadIota = 141,
		DeadVoicedSound = 142,
		DeadSemivoicedSound = 143,
		NumLock = 144,
		ScrollLock = 145,
		Ampersand = 150,
		Asterisk = 151,
		DoubleQuote = 152,
		Less = 153,
		PrintScreen = 154,
		Insert = 155,
		Help = 156,
		Meta = 157,
		Greater = 160,
		LeftBrace = 161,
		RightBrace = 162,
		BackQuote = 192,
		Quote = 222,
		Alphanumeric = 240,
		Katakana = 241,
		Hiragana = 242,
		FullWidth = 243,
		HalfWidth = 244,
		RomanCharacters = 245,
		AllCandidates = 256,
		PreviousCandidate = 257,
		CodeInput = 258,
		JapaneseKatakana = 259,
		JapaneseHiragana = 260,
		JapaneseRoman = 261,
		KanaLock = 262,
		InputMethodOnOff = 263,
		At = 512,
		Colon = 513,
		Circumflex = 514,
		Dollar = 515,
		Euro = 516,
		ExclamationMark = 517,
		InvertedExclamationMark = 518,
		LeftParenthesis = 519,
		NumberSign = 520,
		Plus = 521,
		RightParenthesis = 522,
		Underscore = 523,
		Windows = 524,
		ContextMenu = 525,
	};


	enum class KeyboardKeyType : uint8_t {
		Text,
		Backspace,
		Enter,
		Shift,
		Characters,
		Layout
	};

	class Keyboard;

	class KeyboardKey {
		public:
			KeyboardKey(
				KeyboardKeyType keyType,
				KeyCode code,
				const wchar_t* name,
				float width
			);

			KeyboardKeyType getKeyType() const;
			KeyCode getCode() const;
			const wchar_t* getName() const;

			float getWidth() const;

		private:
			float _width;
			KeyboardKeyType _keyType;
			KeyCode _code;
			const wchar_t* _name;
	};

	class TextKeyboardKey : public KeyboardKey {
		public:
			TextKeyboardKey(
				KeyCode code,
				const wchar_t* name,
				KeyCode uppercaseCode,
				const wchar_t* uppercaseName,
				float width
			);

			KeyCode getUppercaseCode() const;
			const wchar_t* getUppercaseName() const;

		private:
			KeyCode _uppercaseCode;
			const wchar_t* _uppercaseName;
	};

	class KeyboardUIRow : public Layout {
		public:
			KeyboardUIRow(Keyboard* keyboard);

			~KeyboardUIRow();

			Keyboard* getKeyboard() const;

		protected:
			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;

		private:
			Keyboard* _keyboard;
	};

	class KeyboardLayoutRow {
		public:
			KeyboardLayoutRow();

			std::vector<KeyboardKey*> keys;
	};

	class KeyboardLayout {
		public:
			KeyboardLayout(
				const wchar_t* name,
				const wchar_t* nameAbbreviated
			);

			virtual ~KeyboardLayout();

			std::vector<KeyboardLayoutRow*> rows {};

		private:
			const wchar_t* _name;
			const wchar_t* _nameAbbreviated;
	};

	class KeyboardButton : public Button {
		public:
			KeyboardButton(Keyboard* keyboard, uint8_t rowIndex, uint8_t columnIndex);

			Keyboard* getKeyboard();
			KeyboardKey* getKey();

			uint8_t getRowIndex() const;

			uint8_t getKeyIndex() const;

		protected:
			void onClick() override;

		private:
			uint8_t _rowIndex;
			uint8_t _keyIndex;

			Keyboard* _keyboard;
	};

	class Keyboard : public Layout, public FontElement {
		public:
			Keyboard(std::vector<std::function<KeyboardLayout*()>> layoutBuilders);

			~Keyboard();

			void setLayoutIndex(int8_t value);

			int8_t getLayoutIndex();

			KeyboardLayout* getLayout() const;

			const Color* getBackgroundColor() const;

			void setBackgroundColor(const Color* value);

			const Color* getTextButtonPrimaryColor() const;

			void setTextButtonPrimaryColor(const Color* value);

			const Color* getTextButtonSecondaryColor() const;

			void setTextButtonSecondaryColor(const Color* value);

			const Color* getActionButtonPrimaryColor() const;

			void setActionButtonPrimaryColor(const Color* value);

			const Color* getActionButtonSecondaryColor() const;

			void setActionButtonSecondaryColor(const Color* actionButtonSecondaryColor);

			uint8_t getHorizontalKeySpacing() const;
			void setHorizontalKeySpacing(uint8_t horizontalKeySpacing);

			uint8_t getVerticalKeySpacing() const;
			void setVerticalKeySpacing(uint8_t value);

			void setKeyHeight(float keyHeight);

			float getKeyHeight() const;

			Callback<KeyboardKey*>& getOnKeyDown();

		private:
			const Color* _textButtonPrimaryColor = nullptr;
			const Color* _textButtonSecondaryColor = nullptr;
			const Color* _actionButtonPrimaryColor = nullptr;
			const Color* _actionButtonSecondaryColor = nullptr;

			int8_t _layoutIndex = -1;
			KeyboardLayout* _layout = nullptr;
			std::vector<std::function<KeyboardLayout*()>> _layoutBuilders;

			Rectangle _backgroundPanel = Rectangle();
			StackLayout _rowsLayout = StackLayout();

			uint8_t _horizontalKeySpacing = 2;
			float _keyHeight = 1.f / 10.f;

			Callback<KeyboardKey*> _onKeyDown {};

			void deleteLayoutAndUIElements();
	};

	class KeyboardRootLayout : public Layout {
		protected:
			Size onMeasure(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;
	};

	// --

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			// Row 1
			KeyboardKey _keyQ = TextKeyboardKey(KeyCode::Q, L"q", KeyCode::Q, L"Q", 1.f / 10.f);
			KeyboardKey _keyW = TextKeyboardKey(KeyCode::W, L"w", KeyCode::W, L"W", 1.f / 10.f);
			KeyboardKey _keyE = TextKeyboardKey(KeyCode::E, L"e", KeyCode::E, L"E", 1.f / 10.f);
			KeyboardKey _keyR = TextKeyboardKey(KeyCode::R, L"r", KeyCode::R, L"R", 1.f / 10.f);
			KeyboardKey _keyT = TextKeyboardKey(KeyCode::T, L"t", KeyCode::T, L"T", 1.f / 10.f);

			KeyboardKey _keyY = TextKeyboardKey(KeyCode::Y, L"y", KeyCode::Y, L"Y", 1.f / 10.f);
			KeyboardKey _keyU = TextKeyboardKey(KeyCode::U, L"u", KeyCode::U, L"U", 1.f / 10.f);
			KeyboardKey _keyI = TextKeyboardKey(KeyCode::I, L"i", KeyCode::I, L"I", 1.f / 10.f);
			KeyboardKey _keyO = TextKeyboardKey(KeyCode::O, L"o", KeyCode::O, L"O", 1.f / 10.f);
			KeyboardKey _keyP = TextKeyboardKey(KeyCode::P, L"p", KeyCode::P, L"P", 1.f / 10.f);
			KeyboardLayoutRow _row0 = KeyboardLayoutRow();

			// Row 2
			KeyboardKey _keyA = TextKeyboardKey(KeyCode::A, L"a", KeyCode::A, L"A", 1.f / 10.f);
			KeyboardKey _keyS = TextKeyboardKey(KeyCode::S, L"s", KeyCode::S, L"S", 1.f / 10.f);
			KeyboardKey _keyD = TextKeyboardKey(KeyCode::D, L"d", KeyCode::D, L"D", 1.f / 10.f);
			KeyboardKey _keyF = TextKeyboardKey(KeyCode::F, L"f", KeyCode::F, L"F", 1.f / 10.f);
			KeyboardKey _keyG = TextKeyboardKey(KeyCode::G, L"g", KeyCode::G, L"G", 1.f / 10.f);

			KeyboardKey _keyH = TextKeyboardKey(KeyCode::H, L"h", KeyCode::H, L"H", 1.f / 10.f);
			KeyboardKey _keyJ = TextKeyboardKey(KeyCode::J, L"j", KeyCode::J, L"J", 1.f / 10.f);
			KeyboardKey _keyK = TextKeyboardKey(KeyCode::K, L"k", KeyCode::K, L"K", 1.f / 10.f);
			KeyboardKey _keyL = TextKeyboardKey(KeyCode::L, L"l", KeyCode::L, L"L", 1.f / 10.f);
			KeyboardLayoutRow _row1 = KeyboardLayoutRow();

			// Row 3
			KeyboardKey _keyShift = KeyboardKey(KeyboardKeyType::Shift, KeyCode::Shift, L"^", 1.5f / 10.f);
			KeyboardKey _keyZ = TextKeyboardKey(KeyCode::Z, L"z", KeyCode::Z, L"Z", 1.f / 10.f);
			KeyboardKey _keyX = TextKeyboardKey(KeyCode::X, L"x", KeyCode::X, L"X", 1.f / 10.f);
			KeyboardKey _keyC = TextKeyboardKey(KeyCode::C, L"c", KeyCode::C, L"C", 1.f / 10.f);
			KeyboardKey _keyV = TextKeyboardKey(KeyCode::V, L"v", KeyCode::V, L"V", 1.f / 10.f);

			KeyboardKey _keyB = TextKeyboardKey(KeyCode::B, L"b", KeyCode::B, L"B", 1.f / 10.f);
			KeyboardKey _keyN = TextKeyboardKey(KeyCode::N, L"n", KeyCode::N, L"N", 1.f / 10.f);
			KeyboardKey _keyM = TextKeyboardKey(KeyCode::M, L"m", KeyCode::M, L"M", 1.f / 10.f);
			KeyboardKey _keyBackspace = KeyboardKey(KeyboardKeyType::Backspace, KeyCode::Backspace, L"<", 1.5f / 10.f);
			KeyboardLayoutRow _row2 = KeyboardLayoutRow();

			// Row 4
			KeyboardKey _keyCharacters = KeyboardKey(KeyboardKeyType::Characters, KeyCode::None, L"123", 1.f / 10.f);
			KeyboardKey _keyLayout = KeyboardKey(KeyboardKeyType::Layout, KeyCode::None, L"Lang", 1.f / 10.f);
			KeyboardKey _keyComma = KeyboardKey(KeyboardKeyType::Text, KeyCode::Comma, L",", 1.f / 10.f);
			KeyboardKey _keySpace = KeyboardKey(KeyboardKeyType::Text, KeyCode::Space, L"Space", 4.f / 10.f);
			KeyboardKey _keyPeriod = KeyboardKey(KeyboardKeyType::Text, KeyCode::Period, L".", 1.f / 10.f);
			KeyboardKey _keyEnter = KeyboardKey(KeyboardKeyType::Enter, KeyCode::Enter, L"Enter", 2.f / 10.f);
			KeyboardLayoutRow _row3 = KeyboardLayoutRow();
	};
}