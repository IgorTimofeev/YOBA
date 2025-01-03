#pragma once

#include <optional>
#include <cstdint>
#include "container.h"
#include "fontElement.h"
#include "button.h"
#include "rectangle.h"
#include "stackContainer.h"

namespace yoba {
	enum class KeyCode : uint16_t {
		None = 0xFFFF,

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
		QuestionMark = 63,
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

	class Keyboard;
	class KeyboardLayout;
	class KeyboardButton;

	enum class KeyboardKeyType : uint8_t {
		Default,
		Action
	};

	enum class KeyboardCase : uint8_t {
		Lower,
		Upper,
		Caps
	};

	class KeyboardKey {
		public:
			KeyboardKey(
				KeyboardKeyType type,
				KeyCode code,
				const std::wstring_view& name,
				float width
			);

			virtual KeyCode getCodeFromCase(Keyboard* keyboard) const;
			virtual const std::wstring_view& getNameFromCase(Keyboard* keyboard) const;

			virtual void tick(KeyboardButton* button);

			virtual void onPressedChanged(KeyboardButton* button);

			float getWidth() const;
			KeyboardKeyType getType() const;

			KeyCode getCode() const;
			const std::wstring_view& getName() const;

		private:
			KeyboardKeyType _type;
			float _width;
			KeyCode _code;
			const std::wstring_view _name;
	};

	class ContinuousTypingKeyboardKey : public KeyboardKey {
		public:
			ContinuousTypingKeyboardKey(KeyboardKeyType type, KeyCode code, const std::wstring_view& name, float width);

			void tick(KeyboardButton* button) override;

			void onPressedChanged(KeyboardButton* button) override;

		private:
			uint32_t _continuousTypingTime = 0;
	};

	class TextKeyboardKey : public ContinuousTypingKeyboardKey {
		public:
			TextKeyboardKey(
				KeyCode code,
				const std::wstring_view& name,
				KeyCode uppercaseCode,
				const std::wstring_view& uppercaseName,
				float width
			);

			TextKeyboardKey(
				KeyCode code,
				const std::wstring_view& name,
				float width
			);

			KeyCode getUppercaseCode() const;
			const std::wstring_view& getUppercaseName() const;

			KeyCode getCodeFromCase(Keyboard* keyboard) const override;

			const std::wstring_view& getNameFromCase(Keyboard* keyboard) const override;

			void onPressedChanged(KeyboardButton* button) override;

		private:
			KeyCode _uppercaseCode;
			const std::wstring_view _uppercaseName;
	};

	class ShiftKeyboardKey : public KeyboardKey {
		public:
			ShiftKeyboardKey(const std::wstring_view& name, const std::wstring_view& uppercaseName, const std::wstring_view& capsName, float width);

			void onPressedChanged(KeyboardButton* button) override;

			const std::wstring_view& getNameFromCase(Keyboard* keyboard) const override;

		private:
			const std::wstring_view _uppercaseName;
			const std::wstring_view _capsName;
	};

	class BackspaceKeyboardKey : public ContinuousTypingKeyboardKey {
		public:
			BackspaceKeyboardKey(const std::wstring_view& name, float width);
	};

	class EnterKeyboardKey : public KeyboardKey {
		public:
			EnterKeyboardKey(const std::wstring_view& name, float width);
	};

	class CharactersLayoutKeyboardKey : public KeyboardKey {
		public:
			CharactersLayoutKeyboardKey(const std::wstring_view& name, float width, const std::function<KeyboardLayout*()>& layoutBuilder);

			void onPressedChanged(KeyboardButton* button) override;

		private:
			std::function<KeyboardLayout*()> _layoutBuilder;
	};

	class CurrentCyclicLayoutKeyboardKey : public KeyboardKey {
		public:
			CurrentCyclicLayoutKeyboardKey(const std::wstring_view& name, float width);

			void onPressedChanged(KeyboardButton* button) override;
	};

	class CyclicLayoutKeyboardKey : public KeyboardKey {
		public:
			CyclicLayoutKeyboardKey(const std::wstring_view& name, float width);

			void onPressedChanged(KeyboardButton* button) override;
	};

	class KeyboardButtonsRowContainer : public Container {
		public:
			KeyboardButtonsRowContainer(Keyboard* keyboard);

			Keyboard* getKeyboard() const;

		protected:
			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override;

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
			std::vector<KeyboardLayoutRow*> rows {};
	};

	class KeyboardButton : public Button {
		public:
			KeyboardButton(Keyboard* keyboard, KeyboardKey* key);

			void tick() override;

			Keyboard* getKeyboard();
			KeyboardKey* getKey();

			void updateFromCase();

		protected:
			void onPressedChanged() override;

		private:
			KeyboardKey* _key;
			Keyboard* _keyboard;

			void updateTextFromCase();
	};

	class Keyboard : public Container, public FontElement {
		public:
			Keyboard(std::vector<std::function<KeyboardLayout*()>> cyclicLayoutBuilders);

			~Keyboard();

			void setLayout(KeyboardLayout* value);
			KeyboardLayout* getLayout() const;

			uint8_t getCyclicLayoutsCount();
			int8_t getCyclicLayoutIndex();
			void setCyclicLayoutIndex(int8_t value);

			void setNextCyclicLayoutIndex();

			const Color* getBackgroundColor() const;

			void setBackgroundColor(const Color* value);

			const Color* getDefaultButtonPrimaryColor() const;

			void setDefaultButtonPrimaryColor(const Color* value);

			const Color* getDefaultButtonSecondaryColor() const;

			void setDefaultButtonSecondaryColor(const Color* value);

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

			KeyboardCase getCase() const;
			void setCase(KeyboardCase value);

			Callback<KeyCode, bool>& getOnKeyPressedChanged();
			Callback<KeyCode, const std::wstring_view&>& getOnInput();

			uint16_t getContinuousTypingDelay() const;
			void setContinuousTypingDelay(uint16_t value);

			uint16_t getContinuousTypingInterval() const;
			void setContinuousTypingInterval(uint16_t value);

		private:
			const Color* _defaultButtonPrimaryColor = nullptr;
			const Color* _defaultButtonSecondaryColor = nullptr;
			const Color* _actionButtonPrimaryColor = nullptr;
			const Color* _actionButtonSecondaryColor = nullptr;

			KeyboardCase _case = KeyboardCase::Lower;

			uint16_t _continuousTypingDelay = 500;
			uint16_t _continuousTypingInterval = 50;

			int8_t _cyclicLayoutIndex = -1;
			KeyboardLayout* _layout = nullptr;
			std::vector<std::function<KeyboardLayout*()>> _cyclicLayoutBuilders;

			Rectangle _backgroundPanel = Rectangle();
			StackContainer _rowsLayout = StackContainer();

			uint8_t _horizontalKeySpacing = 2;
			float _keyHeight = 0.1f;

			Callback<KeyCode, bool> _onKeyPressedChanged {};
			Callback<KeyCode, const std::wstring_view&> _onInput {};

			void deleteLayoutAndUIElements();

			void iterateOverButtons(std::function<void(KeyboardButton*)> handler);
	};

	class KeyboardApplicationContainer : public Container {
		protected:
			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;
	};

	// ----------------------------- Layouts -----------------------------

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardKey _key1 = TextKeyboardKey(KeyCode::Number1, L"1", 0.1f);
			TextKeyboardKey _key2 = TextKeyboardKey(KeyCode::Number2, L"2", 0.1f);
			TextKeyboardKey _key3 = TextKeyboardKey(KeyCode::Number3, L"3", 0.1f);
			TextKeyboardKey _key4 = TextKeyboardKey(KeyCode::Number4, L"4", 0.1f);
			TextKeyboardKey _key5 = TextKeyboardKey(KeyCode::Number5, L"5", 0.1f);
			TextKeyboardKey _key6 = TextKeyboardKey(KeyCode::Number6, L"6", 0.1f);
			TextKeyboardKey _key7 = TextKeyboardKey(KeyCode::Number7, L"7", 0.1f);
			TextKeyboardKey _key8 = TextKeyboardKey(KeyCode::Number8, L"8", 0.1f);
			TextKeyboardKey _key9 = TextKeyboardKey(KeyCode::Number9, L"9", 0.1f);
			TextKeyboardKey _key0 = TextKeyboardKey(KeyCode::Number0, L"0", 0.1f);
			KeyboardLayoutRow _row0 = KeyboardLayoutRow();

			TextKeyboardKey _keyAt = TextKeyboardKey(KeyCode::At, L"@", 0.1f);
			TextKeyboardKey _keyNumberSign = TextKeyboardKey(KeyCode::NumberSign, L"#", 0.1f);
			TextKeyboardKey _keyDollar = TextKeyboardKey(KeyCode::Dollar, L"$", 0.1f);
			TextKeyboardKey _keyUnderscore = TextKeyboardKey(KeyCode::Underscore, L"_", 0.1f);
			TextKeyboardKey _keyAmpersand = TextKeyboardKey(KeyCode::Ampersand, L"&", 0.1f);
			TextKeyboardKey _keyMinus = TextKeyboardKey(KeyCode::Minus, L"-", 0.1f);
			TextKeyboardKey _keyPlus = TextKeyboardKey(KeyCode::Plus, L"+", 0.1f);
			TextKeyboardKey _keyLeftBrace = TextKeyboardKey(KeyCode::LeftBrace, L"(", 0.1f);
			TextKeyboardKey _keyRightBrace = TextKeyboardKey(KeyCode::RightBrace, L")", 0.1f);
			TextKeyboardKey _keySlash = TextKeyboardKey(KeyCode::Slash, L"/", 0.1f);
			KeyboardLayoutRow _row1 = KeyboardLayoutRow();

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyAsterisk = TextKeyboardKey(KeyCode::Asterisk, L"*", 0.1f);
			TextKeyboardKey _keyDoubleQuote = TextKeyboardKey(KeyCode::DoubleQuote, L"\"", 0.1f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(KeyCode::Quote, L"\'", 0.1f);
			TextKeyboardKey _keyColon = TextKeyboardKey(KeyCode::Colon, L":", 0.1f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(KeyCode::Semicolon, L";", 0.1f);
			TextKeyboardKey _keyExclamationMark = TextKeyboardKey(KeyCode::ExclamationMark, L"!", 0.1f);
			TextKeyboardKey _keyQuestionMark = TextKeyboardKey(KeyCode::QuestionMark, L"?", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);
			KeyboardLayoutRow _row2 = KeyboardLayoutRow();

			CurrentCyclicLayoutKeyboardKey _keyCyclicLayout = CurrentCyclicLayoutKeyboardKey(L"Abc", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::Comma, L",", 0.1f);
			TextKeyboardKey _keySpace = TextKeyboardKey(KeyCode::Space, L" ", 0.5f);
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::Period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(L"Enter", 0.2f);
			KeyboardLayoutRow _row3 = KeyboardLayoutRow();
	};

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			TextKeyboardKey _keyQ = TextKeyboardKey(KeyCode::Q, L"q", KeyCode::Q, L"Q", 0.1f);
			TextKeyboardKey _keyW = TextKeyboardKey(KeyCode::W, L"w", KeyCode::W, L"W", 0.1f);
			TextKeyboardKey _keyE = TextKeyboardKey(KeyCode::E, L"e", KeyCode::E, L"E", 0.1f);
			TextKeyboardKey _keyR = TextKeyboardKey(KeyCode::R, L"r", KeyCode::R, L"R", 0.1f);
			TextKeyboardKey _keyT = TextKeyboardKey(KeyCode::T, L"t", KeyCode::T, L"T", 0.1f);
			TextKeyboardKey _keyY = TextKeyboardKey(KeyCode::Y, L"y", KeyCode::Y, L"Y", 0.1f);
			TextKeyboardKey _keyU = TextKeyboardKey(KeyCode::U, L"u", KeyCode::U, L"L", 0.1f);
			TextKeyboardKey _keyI = TextKeyboardKey(KeyCode::I, L"i", KeyCode::I, L"I", 0.1f);
			TextKeyboardKey _keyO = TextKeyboardKey(KeyCode::O, L"o", KeyCode::O, L"O", 0.1f);
			TextKeyboardKey _keyP = TextKeyboardKey(KeyCode::P, L"p", KeyCode::P, L"P", 0.1f);
			KeyboardLayoutRow _row0 = KeyboardLayoutRow();

			TextKeyboardKey _keyA = TextKeyboardKey(KeyCode::A, L"a", KeyCode::A, L"A", 0.1f);
			TextKeyboardKey _keyS = TextKeyboardKey(KeyCode::S, L"s", KeyCode::S, L"S", 0.1f);
			TextKeyboardKey _keyD = TextKeyboardKey(KeyCode::D, L"d", KeyCode::D, L"D", 0.1f);
			TextKeyboardKey _keyF = TextKeyboardKey(KeyCode::F, L"f", KeyCode::F, L"F", 0.1f);
			TextKeyboardKey _keyG = TextKeyboardKey(KeyCode::G, L"g", KeyCode::G, L"G", 0.1f);
			TextKeyboardKey _keyH = TextKeyboardKey(KeyCode::H, L"h", KeyCode::H, L"H", 0.1f);
			TextKeyboardKey _keyJ = TextKeyboardKey(KeyCode::J, L"j", KeyCode::J, L"J", 0.1f);
			TextKeyboardKey _keyK = TextKeyboardKey(KeyCode::K, L"k", KeyCode::K, L"K", 0.1f);
			TextKeyboardKey _keyL = TextKeyboardKey(KeyCode::L, L"l", KeyCode::L, L"L", 0.1f);
			KeyboardLayoutRow _row1 = KeyboardLayoutRow();

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyZ = TextKeyboardKey(KeyCode::Z, L"z", KeyCode::Z, L"Z", 0.1f);
			TextKeyboardKey _keyX = TextKeyboardKey(KeyCode::X, L"x", KeyCode::X, L"X", 0.1f);
			TextKeyboardKey _keyC = TextKeyboardKey(KeyCode::C, L"c", KeyCode::C, L"C", 0.1f);
			TextKeyboardKey _keyV = TextKeyboardKey(KeyCode::V, L"v", KeyCode::V, L"V", 0.1f);
			TextKeyboardKey _keyB = TextKeyboardKey(KeyCode::B, L"b", KeyCode::B, L"B", 0.1f);
			TextKeyboardKey _keyN = TextKeyboardKey(KeyCode::N, L"n", KeyCode::N, L"N", 0.1f);
			TextKeyboardKey _keyM = TextKeyboardKey(KeyCode::M, L"m", KeyCode::M, L"M", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);
			KeyboardLayoutRow _row2 = KeyboardLayoutRow();

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f, []() {
				return new CharactersKeyboardLayout();
			});

			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::Comma, L",", 0.1f);
			TextKeyboardKey _keySpace = TextKeyboardKey(KeyCode::Space, L" ", 0.4f);
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::Period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey( L"Enter", 0.2f);
			KeyboardLayoutRow _row3 = KeyboardLayoutRow();
	};
}