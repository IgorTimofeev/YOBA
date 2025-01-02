#pragma once

#include <optional>
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

	enum class KeyboardKeyStyle : uint8_t {
		Default,
		Action
	};

	enum class KeyboardCase : uint8_t {
		Lower,
		Upper,
		Caps
	};

	class KeyboardKeyModel {
		public:
			KeyboardKeyModel(
				KeyboardKeyStyle style,
				KeyCode code,
				const std::wstring_view& name,
				float width
			);

			KeyCode getCode() const;
			const std::wstring_view& getName() const;

			float getWidth() const;

			KeyboardKeyStyle getStyle() const;

			virtual void onClick(Keyboard* keyboard);

			virtual KeyCode getCodeFromCase(Keyboard* keyboard) const;

			virtual const std::wstring_view& getNameFromCase(Keyboard* keyboard) const;

		private:
			KeyboardKeyStyle _style;
			float _width;
			KeyCode _code;
			const std::wstring_view _name;
	};

	class TextKeyboardKeyModel : public KeyboardKeyModel {
		public:
			TextKeyboardKeyModel(
				KeyCode code,
				const std::wstring_view& name,
				KeyCode uppercaseCode,
				const std::wstring_view& uppercaseName,
				float width
			);

			TextKeyboardKeyModel(
				KeyCode code,
				const std::wstring_view& name,
				float width
			);

			KeyCode getUppercaseCode() const;
			const std::wstring_view& getUppercaseName() const;

			KeyCode getCodeFromCase(Keyboard* keyboard) const override;

			const std::wstring_view& getNameFromCase(Keyboard* keyboard) const override;

			void onClick(Keyboard* keyboard) override;



		private:
			KeyCode _uppercaseCode;
			const std::wstring_view _uppercaseName;
	};

	class ActionKeyboardKeyModel : public KeyboardKeyModel {
		public:
			ActionKeyboardKeyModel(KeyCode code, const std::wstring_view& name, float width);

	};

	class ShiftKeyboardKeyModel : public ActionKeyboardKeyModel {
		public:
			ShiftKeyboardKeyModel(const std::wstring_view& name, const std::wstring_view& uppercaseName, const std::wstring_view& capsName, float width);

			void onClick(Keyboard* keyboard) override;

			const std::wstring_view& getNameFromCase(Keyboard* keyboard) const override;

		private:
			const std::wstring_view _uppercaseName;
			const std::wstring_view _capsName;
	};

	class KeyboardButtonsRow : public Container {
		public:
			KeyboardButtonsRow(Keyboard* keyboard);

			~KeyboardButtonsRow();

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

			std::vector<KeyboardKeyModel*> keys;
	};

	class KeyboardLayout {
		public:
			std::vector<KeyboardLayoutRow*> rows {};
	};

	class KeyboardButton : public Button {
		public:
			KeyboardButton(Keyboard* keyboard, KeyboardKeyModel* model);

			Keyboard* getKeyboard();
			KeyboardKeyModel* getModel();

			void updateFromCase();

		protected:
			void onClick() override;

		private:
			KeyboardKeyModel* _model;
			Keyboard* _keyboard;

			void updateTextFromCase();
	};

	class Keyboard : public Container, public FontElement {
		public:
			Keyboard(std::vector<std::function<KeyboardLayout*()>> layoutBuilders);

			~Keyboard();

			void setLayoutIndex(int8_t value);

			int8_t getLayoutIndex();

			KeyboardLayout* getLayout() const;

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

			Callback<KeyCode>& getOnKeyDown();
			Callback<KeyCode, const std::wstring_view&>& getOnInput();

		private:
			const Color* _defaultButtonPrimaryColor = nullptr;
			const Color* _defaultButtonSecondaryColor = nullptr;
			const Color* _actionButtonPrimaryColor = nullptr;
			const Color* _actionButtonSecondaryColor = nullptr;

			KeyboardCase _case = KeyboardCase::Lower;
			int8_t _layoutIndex = -1;
			KeyboardLayout* _layout = nullptr;
			std::vector<std::function<KeyboardLayout*()>> _layoutBuilders;

			Rectangle _backgroundPanel = Rectangle();
			StackContainer _rowsLayout = StackContainer();

			uint8_t _horizontalKeySpacing = 2;
			float _keyHeight = 0.1f;

			Callback<KeyCode> _onKeyDown {};
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

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			TextKeyboardKeyModel _keyQ = TextKeyboardKeyModel(KeyCode::Q, L"q", KeyCode::Q, L"Q", 0.1f);
			TextKeyboardKeyModel _keyW = TextKeyboardKeyModel(KeyCode::W, L"w", KeyCode::W, L"W", 0.1f);
			TextKeyboardKeyModel _keyE = TextKeyboardKeyModel(KeyCode::E, L"e", KeyCode::E, L"E", 0.1f);
			TextKeyboardKeyModel _keyR = TextKeyboardKeyModel(KeyCode::R, L"r", KeyCode::R, L"R", 0.1f);
			TextKeyboardKeyModel _keyT = TextKeyboardKeyModel(KeyCode::T, L"t", KeyCode::T, L"T", 0.1f);
			TextKeyboardKeyModel _keyY = TextKeyboardKeyModel(KeyCode::Y, L"y", KeyCode::Y, L"Y", 0.1f);
			TextKeyboardKeyModel _keyU = TextKeyboardKeyModel(KeyCode::U, L"u", KeyCode::U, L"L", 0.1f);
			TextKeyboardKeyModel _keyI = TextKeyboardKeyModel(KeyCode::I, L"i", KeyCode::I, L"I", 0.1f);
			TextKeyboardKeyModel _keyO = TextKeyboardKeyModel(KeyCode::O, L"o", KeyCode::O, L"O", 0.1f);
			TextKeyboardKeyModel _keyP = TextKeyboardKeyModel(KeyCode::P, L"p", KeyCode::P, L"P", 0.1f);
			KeyboardLayoutRow _row0 = KeyboardLayoutRow();

			TextKeyboardKeyModel _keyA = TextKeyboardKeyModel(KeyCode::A, L"a", KeyCode::A, L"A", 0.1f);
			TextKeyboardKeyModel _keyS = TextKeyboardKeyModel(KeyCode::S, L"s", KeyCode::S, L"S", 0.1f);
			TextKeyboardKeyModel _keyD = TextKeyboardKeyModel(KeyCode::D, L"d", KeyCode::D, L"D", 0.1f);
			TextKeyboardKeyModel _keyF = TextKeyboardKeyModel(KeyCode::F, L"f", KeyCode::F, L"F", 0.1f);
			TextKeyboardKeyModel _keyG = TextKeyboardKeyModel(KeyCode::G, L"g", KeyCode::G, L"G", 0.1f);
			TextKeyboardKeyModel _keyH = TextKeyboardKeyModel(KeyCode::H, L"h", KeyCode::H, L"H", 0.1f);
			TextKeyboardKeyModel _keyJ = TextKeyboardKeyModel(KeyCode::J, L"j", KeyCode::J, L"J", 0.1f);
			TextKeyboardKeyModel _keyK = TextKeyboardKeyModel(KeyCode::K, L"k", KeyCode::K, L"K", 0.1f);
			TextKeyboardKeyModel _keyL = TextKeyboardKeyModel(KeyCode::L, L"l", KeyCode::L, L"L", 0.1f);
			KeyboardLayoutRow _row1 = KeyboardLayoutRow();

			ShiftKeyboardKeyModel _keyShift = ShiftKeyboardKeyModel(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKeyModel _keyZ = TextKeyboardKeyModel(KeyCode::Z, L"z", KeyCode::Z, L"Z", 0.1f);
			TextKeyboardKeyModel _keyX = TextKeyboardKeyModel(KeyCode::X, L"x", KeyCode::X, L"X", 0.1f);
			TextKeyboardKeyModel _keyC = TextKeyboardKeyModel(KeyCode::C, L"c", KeyCode::C, L"C", 0.1f);
			TextKeyboardKeyModel _keyV = TextKeyboardKeyModel(KeyCode::V, L"v", KeyCode::V, L"V", 0.1f);
			TextKeyboardKeyModel _keyB = TextKeyboardKeyModel(KeyCode::B, L"b", KeyCode::B, L"B", 0.1f);
			TextKeyboardKeyModel _keyN = TextKeyboardKeyModel(KeyCode::N, L"n", KeyCode::N, L"N", 0.1f);
			TextKeyboardKeyModel _keyM = TextKeyboardKeyModel(KeyCode::M, L"m", KeyCode::M, L"M", 0.1f);
			ActionKeyboardKeyModel _keyBackspace = ActionKeyboardKeyModel(KeyCode::Backspace, L"<", 0.15f);
			KeyboardLayoutRow _row2 = KeyboardLayoutRow();

			ActionKeyboardKeyModel _keyCharacters = ActionKeyboardKeyModel(KeyCode::None, L"123", 0.1f);
			ActionKeyboardKeyModel _keyLayout = ActionKeyboardKeyModel(KeyCode::None, L"Lang", 0.1f);
			TextKeyboardKeyModel _keyComma = TextKeyboardKeyModel(KeyCode::Comma, L",", 0.1f);
			TextKeyboardKeyModel _keySpace = TextKeyboardKeyModel(KeyCode::Space, L" ", 0.4f);
			TextKeyboardKeyModel _keyPeriod = TextKeyboardKeyModel(KeyCode::Period, L".", 0.1f);
			ActionKeyboardKeyModel _keyEnter = ActionKeyboardKeyModel(KeyCode::Enter, L"Enter", 0.2f);
			KeyboardLayoutRow _row3 = KeyboardLayoutRow();
	};

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardKeyModel _key1 = TextKeyboardKeyModel(KeyCode::Number1, L"1", 0.1f);
			TextKeyboardKeyModel _key2 = TextKeyboardKeyModel(KeyCode::Number2, L"2", 0.1f);
			TextKeyboardKeyModel _key3 = TextKeyboardKeyModel(KeyCode::Number3, L"3", 0.1f);
			TextKeyboardKeyModel _key4 = TextKeyboardKeyModel(KeyCode::Number4, L"4", 0.1f);
			TextKeyboardKeyModel _key5 = TextKeyboardKeyModel(KeyCode::Number5, L"5", 0.1f);
			TextKeyboardKeyModel _key6 = TextKeyboardKeyModel(KeyCode::Number6, L"6", 0.1f);
			TextKeyboardKeyModel _key7 = TextKeyboardKeyModel(KeyCode::Number7, L"7", 0.1f);
			TextKeyboardKeyModel _key8 = TextKeyboardKeyModel(KeyCode::Number8, L"8", 0.1f);
			TextKeyboardKeyModel _key9 = TextKeyboardKeyModel(KeyCode::Number9, L"9", 0.1f);
			TextKeyboardKeyModel _key0 = TextKeyboardKeyModel(KeyCode::Number0, L"0", 0.1f);
			KeyboardLayoutRow _row0 = KeyboardLayoutRow();

			TextKeyboardKeyModel _keyAt = TextKeyboardKeyModel(KeyCode::At, L"@", 0.1f);
			TextKeyboardKeyModel _keyNumberSign = TextKeyboardKeyModel(KeyCode::NumberSign, L"#", 0.1f);
			TextKeyboardKeyModel _keyDollar = TextKeyboardKeyModel(KeyCode::Dollar, L"$", 0.1f);
			TextKeyboardKeyModel _keyUnderscore = TextKeyboardKeyModel(KeyCode::Underscore, L"_", 0.1f);
			TextKeyboardKeyModel _keyAmpersand = TextKeyboardKeyModel(KeyCode::Ampersand, L"&", 0.1f);
			TextKeyboardKeyModel _keyMinus = TextKeyboardKeyModel(KeyCode::Minus, L"-", 0.1f);
			TextKeyboardKeyModel _keyPlus = TextKeyboardKeyModel(KeyCode::Plus, L"+", 0.1f);
			TextKeyboardKeyModel _keyLeftBrace = TextKeyboardKeyModel(KeyCode::LeftBrace, L"(", 0.1f);
			TextKeyboardKeyModel _keyRightBrace = TextKeyboardKeyModel(KeyCode::RightBrace, L")", 0.1f);
			TextKeyboardKeyModel _keySlash = TextKeyboardKeyModel(KeyCode::Slash, L"/", 0.1f);
			KeyboardLayoutRow _row1 = KeyboardLayoutRow();

			TextKeyboardKeyModel _keyAsterisk = TextKeyboardKeyModel(KeyCode::Asterisk, L"*", 0.1f);
			TextKeyboardKeyModel _keyDoubleQuote = TextKeyboardKeyModel(KeyCode::DoubleQuote, L"\"", 0.1f);
			TextKeyboardKeyModel _keyQuote = TextKeyboardKeyModel(KeyCode::Quote, L"\'", 0.1f);
			TextKeyboardKeyModel _keyColon = TextKeyboardKeyModel(KeyCode::Colon, L":", 0.1f);
			TextKeyboardKeyModel _keySemicolon = TextKeyboardKeyModel(KeyCode::Semicolon, L";", 0.1f);
			TextKeyboardKeyModel _keyExclamationMark = TextKeyboardKeyModel(KeyCode::ExclamationMark, L"!", 0.1f);
			TextKeyboardKeyModel _keyQuestionMark = TextKeyboardKeyModel(KeyCode::QuestionMark, L"?", 0.1f);
			ActionKeyboardKeyModel _keyBackspace = ActionKeyboardKeyModel(KeyCode::Backspace, L"<", 0.3f);
			KeyboardLayoutRow _row2 = KeyboardLayoutRow();

			ActionKeyboardKeyModel _keyCharacters = ActionKeyboardKeyModel(KeyCode::None, L"Abc", 0.1f);
			ActionKeyboardKeyModel _keyLayout = ActionKeyboardKeyModel(KeyCode::None, L"Lang", 0.1f);
			TextKeyboardKeyModel _keyComma = TextKeyboardKeyModel(KeyCode::Comma, L",", 0.1f);
			TextKeyboardKeyModel _keySpace = TextKeyboardKeyModel(KeyCode::Space, L" ", 0.4f);
			TextKeyboardKeyModel _keyPeriod = TextKeyboardKeyModel(KeyCode::Period, L".", 0.1f);
			ActionKeyboardKeyModel _keyEnter = ActionKeyboardKeyModel(KeyCode::Enter, L"Enter", 0.2f);
			KeyboardLayoutRow _row3 = KeyboardLayoutRow();
	};
}