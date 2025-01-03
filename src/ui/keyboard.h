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
		None,
		Cancel,
		Backspace,
		Tab,
		Enter,
		Clear,
		Shift,
		Ctrl,
		Alt,
		Pause,
		CapsLock,
		Kana,
		Final,
		Kanji,
		Escape,
		Convert,
		NoConvert,
		Accept,
		ModeChange,
		Space,
		PageUp,
		PageDown,
		End,
		Home,
		Left,
		Up,
		Right,
		Down,
		Comma,
		Minus,
		Period,
		Slash,
		Number0,
		Number1,
		Number2,
		Number3,
		Number4,
		Number5,
		Number6,
		Number7,
		Number8,
		Number9,
		Semicolon,
		Equals,
		QuestionMark,
		A,
		B,
		C,
		D,
		E,
		F,
		G,
		H,
		I,
		J,
		K,
		L,
		M,
		N,
		O,
		P,
		Q,
		R,
		S,
		T,
		U,
		V,
		W,
		X,
		Y,
		Z,
		OpenBracket,
		BackSlash,
		CloseBracket,
		Percent,
		Copyright,
		Registered,
		Trademark,
		Check,
		NumPad0,
		NumPad1,
		NumPad2,
		NumPad3,
		NumPad4,
		NumPad5,
		NumPad6,
		NumPad7,
		NumPad8,
		NumPad9,
		NumPadAsterisk,
		NumPadPlus,
		NumPadComma,
		NumPadMinusf,
		NumPadPeriod,
		NumPadSlash,
		F1,
		F2,
		F3,
		F4,
		F5,
		F6,
		F7,
		F8,
		F9,
		F10,
		F11,
		F12,
		Delete,
		VerticalBar,
		Bullet,
		SquareRoot,
		Pi,
		Division,
		Multiplication,
		Section,
		Delta,
		Pound,
		Cent,
		Yen,
		Degree,
		DeadGrave,
		DeadAcute,
		DeadCircumflex,
		DeadTilde,
		DeadMacron,
		DeadBreve,
		DeadAboveDot,
		DeadDiaeresis,
		DeadAboveRing,
		DeadDoubleAcute,
		DeadCaron,
		DeadCedilla,
		DeadOgonek,
		DeadIota,
		DeadVoicedSound,
		DeadSemivoicedSound,
		NumLock,
		ScrollLock,
		Ampersand,
		Asterisk,
		DoubleQuote,
		LessThan,
		MoreThan,
		PrintScreen,
		Insert,
		Help,
		Meta,
		Greater,
		LeftBrace,
		RightBrace,
		BackQuote,
		Quote,
		Alphanumeric,
		Katakana,
		Hiragana,
		FullWidth,
		HalfWidth,
		RomanCharacters,
		AllCandidates,
		PreviousCandidate,
		CodeInput,
		JapaneseKatakana,
		JapaneseHiragana,
		JapaneseRoman,
		KanaLock,
		InputMethodOnOff,
		At,
		Colon,
		Circumflex,
		Dollar,
		Euro,
		ExclamationMark,
		InvertedExclamationMark,
		LeftParenthesis,
		NumberSign,
		Plus,
		RightParenthesis,
		Underscore,
		Windows,
		ContextMenu
	};

	class Keyboard;
	class KeyboardLayout;
	class KeyboardButton;

	enum class KeyboardKeyType : uint8_t {
		Default,
		CharactersLayout,
		CyclicLayout,
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

			static float fit;

			virtual void tick(KeyboardButton* button);
			virtual void onPressedChanged(KeyboardButton* button);
			virtual KeyCode getCodeFromCase(Keyboard* keyboard) const;
			virtual const std::wstring_view& getNameFromCase(Keyboard* keyboard) const;

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

	class TextKeyboardKey : public KeyboardKey {
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

	class BackspaceKeyboardKey : public KeyboardKey {
		public:
			BackspaceKeyboardKey(const std::wstring_view& name, float width);
	};

	class EnterKeyboardKey : public KeyboardKey {
		public:
			EnterKeyboardKey(const std::wstring_view& name, float width);
	};

	class SpaceKeyboardKey : public KeyboardKey {
		public:
			SpaceKeyboardKey();
	};

	class CharactersLayoutKeyboardKey : public KeyboardKey {
		public:
			CharactersLayoutKeyboardKey(const std::wstring_view& name, float width);

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

	class KeyboardButtonsContainer : public Container {
		public:
			explicit KeyboardButtonsContainer(Keyboard* keyboard);

		protected:
			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& buttonIndexTo) override;

		private:
			Keyboard* _keyboard;
	};

	class KeyboardLayout {
		public:
			KeyboardLayout(std::vector<std::vector<KeyboardKey*>> keys) : keys(keys) {

			}

			std::vector<std::vector<KeyboardKey*>> keys;
	};

	class KeyboardButton : public Button {
		public:
			KeyboardButton(Keyboard* keyboard, uint8_t row, uint8_t column);

			void tick() override;

			Keyboard* getKeyboard();
			KeyboardKey* getKey();

			uint8_t getRow() const;
			uint8_t getColumn() const;

			void updateFromCase();

		protected:
			void onPressedChanged() override;

		private:
			uint8_t _row;
			uint8_t _column;
			Keyboard* _keyboard;

			void updateTextFromCase();
	};

	class Keyboard : public Container, public FontElement {
		public:
			Keyboard();

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

			std::vector<std::function<KeyboardLayout*()>>& getCyclicLayoutBuilders();
			void setCyclicLayoutBuilders(const std::vector<std::function<KeyboardLayout*()>>& cyclicLayoutBuilders);

			const std::optional<std::function<KeyboardLayout*()>>& getCharactersLayoutBuilder() const;

			void setCharactersLayoutBuilder(const std::optional<std::function<KeyboardLayout*()>>& charactersLayoutBuilder);

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
			std::optional<std::function<KeyboardLayout*()>> _charactersLayoutBuilder = std::nullopt;

			Rectangle _backgroundPanel = Rectangle();
			KeyboardButtonsContainer _buttonsContainer;

			uint8_t _horizontalKeySpacing = 2;
			uint8_t _verticalKeySpacing = 2;
			float _keyHeight = 0.1f;

			Callback<KeyCode, bool> _onKeyPressedChanged {};
			Callback<KeyCode, const std::wstring_view&> _onInput {};

			void deleteLayoutAndUIElements();
	};

	class KeyboardApplicationContainer : public Container {
		protected:
			Size computeDesiredSize(ScreenBuffer* screenBuffer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;
	};

	class ApplicationKeyboardController {
		public:
			static Keyboard* show(Application* application);
			static void hide();

		private:
			static Keyboard* _keyboard;
			static Container* _applicationChildrenContainer;
			static KeyboardApplicationContainer* _keyboardAndApplicationChildrenContainer;
	};

	// ----------------------------- Layouts -----------------------------

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardKey _key1 = TextKeyboardKey(KeyCode::Number1, L"1", KeyCode::DeadTilde, L"~", 0.1f);
			TextKeyboardKey _key2 = TextKeyboardKey(KeyCode::Number2, L"2", KeyCode::DeadGrave, L"`", 0.1f);
			TextKeyboardKey _key3 = TextKeyboardKey(KeyCode::Number3, L"3", KeyCode::VerticalBar, L"|", 0.1f);
			TextKeyboardKey _key4 = TextKeyboardKey(KeyCode::Number4, L"4", KeyCode::Bullet, L"•", 0.1f);
			TextKeyboardKey _key5 = TextKeyboardKey(KeyCode::Number5, L"5", KeyCode::SquareRoot, L"√", 0.1f);
			TextKeyboardKey _key6 = TextKeyboardKey(KeyCode::Number6, L"6", KeyCode::Pi, L"π", 0.1f);
			TextKeyboardKey _key7 = TextKeyboardKey(KeyCode::Number7, L"7", KeyCode::Division, L"÷", 0.1f);
			TextKeyboardKey _key8 = TextKeyboardKey(KeyCode::Number8, L"8", KeyCode::Multiplication, L"×", 0.1f);
			TextKeyboardKey _key9 = TextKeyboardKey(KeyCode::Number9, L"9", KeyCode::Section, L"§", 0.1f);
			TextKeyboardKey _key0 = TextKeyboardKey(KeyCode::Number0, L"0", KeyCode::Delta, L"Δ", 0.1f);

			TextKeyboardKey _keyAt = TextKeyboardKey(KeyCode::At, L"@", KeyCode::Pound, L"£", 0.1f);
			TextKeyboardKey _keyNumberSign = TextKeyboardKey(KeyCode::NumberSign, L"#", KeyCode::Cent, L"¢", 0.1f);
			TextKeyboardKey _keyDollar = TextKeyboardKey(KeyCode::Dollar, L"$", KeyCode::Euro, L"€", 0.1f);
			TextKeyboardKey _keyUnderscore = TextKeyboardKey(KeyCode::Underscore, L"_", KeyCode::Yen, L"¥", 0.1f);
			TextKeyboardKey _keyAmpersand = TextKeyboardKey(KeyCode::Ampersand, L"&", KeyCode::Circumflex, L"^", 0.1f);
			TextKeyboardKey _keyMinus = TextKeyboardKey(KeyCode::Minus, L"-", KeyCode::Degree, L"°", 0.1f);
			TextKeyboardKey _keyPlus = TextKeyboardKey(KeyCode::Plus, L"+", KeyCode::Equals, L"=", 0.1f);
			TextKeyboardKey _keyLeftBrace = TextKeyboardKey(KeyCode::LeftBrace, L"(", KeyCode::LeftParenthesis, L"{", 0.1f);
			TextKeyboardKey _keyRightBrace = TextKeyboardKey(KeyCode::RightBrace, L")", KeyCode::RightParenthesis, L"}", 0.1f);
			TextKeyboardKey _keySlash = TextKeyboardKey(KeyCode::Slash, L"/", KeyCode::BackSlash, L"\\", 0.1f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyAsterisk = TextKeyboardKey(KeyCode::Asterisk, L"*", KeyCode::Percent, L"%", 0.1f);
			TextKeyboardKey _keyDoubleQuote = TextKeyboardKey(KeyCode::DoubleQuote, L"\"", KeyCode::Copyright, L"©", 0.1f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(KeyCode::Quote, L"\'", KeyCode::Registered, L"®", 0.1f);
			TextKeyboardKey _keyColon = TextKeyboardKey(KeyCode::Colon, L":", KeyCode::Trademark, L"™", 0.1f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(KeyCode::Semicolon, L";", KeyCode::Check, L"✓", 0.1f);
			TextKeyboardKey _keyExclamationMark = TextKeyboardKey(KeyCode::ExclamationMark, L"!", KeyCode::OpenBracket, L"[", 0.1f);
			TextKeyboardKey _keyQuestionMark = TextKeyboardKey(KeyCode::QuestionMark, L"?", KeyCode::CloseBracket, L"]", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);

			CurrentCyclicLayoutKeyboardKey _keyCyclicLayout = CurrentCyclicLayoutKeyboardKey(L"Abc", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::Comma, L",", 0.1f);
			TextKeyboardKey _keySpace = TextKeyboardKey(KeyCode::Space, L" ", 0.5f);
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::Period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(L"Enter", 0.2f);
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

			TextKeyboardKey _keyA = TextKeyboardKey(KeyCode::A, L"a", KeyCode::A, L"A", 0.1f);
			TextKeyboardKey _keyS = TextKeyboardKey(KeyCode::S, L"s", KeyCode::S, L"S", 0.1f);
			TextKeyboardKey _keyD = TextKeyboardKey(KeyCode::D, L"d", KeyCode::D, L"D", 0.1f);
			TextKeyboardKey _keyF = TextKeyboardKey(KeyCode::F, L"f", KeyCode::F, L"F", 0.1f);
			TextKeyboardKey _keyG = TextKeyboardKey(KeyCode::G, L"g", KeyCode::G, L"G", 0.1f);
			TextKeyboardKey _keyH = TextKeyboardKey(KeyCode::H, L"h", KeyCode::H, L"H", 0.1f);
			TextKeyboardKey _keyJ = TextKeyboardKey(KeyCode::J, L"j", KeyCode::J, L"J", 0.1f);
			TextKeyboardKey _keyK = TextKeyboardKey(KeyCode::K, L"k", KeyCode::K, L"K", 0.1f);
			TextKeyboardKey _keyL = TextKeyboardKey(KeyCode::L, L"l", KeyCode::L, L"L", 0.1f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyZ = TextKeyboardKey(KeyCode::Z, L"z", KeyCode::Z, L"Z", 0.1f);
			TextKeyboardKey _keyX = TextKeyboardKey(KeyCode::X, L"x", KeyCode::X, L"X", 0.1f);
			TextKeyboardKey _keyC = TextKeyboardKey(KeyCode::C, L"c", KeyCode::C, L"C", 0.1f);
			TextKeyboardKey _keyV = TextKeyboardKey(KeyCode::V, L"v", KeyCode::V, L"V", 0.1f);
			TextKeyboardKey _keyB = TextKeyboardKey(KeyCode::B, L"b", KeyCode::B, L"B", 0.1f);
			TextKeyboardKey _keyN = TextKeyboardKey(KeyCode::N, L"n", KeyCode::N, L"N", 0.1f);
			TextKeyboardKey _keyM = TextKeyboardKey(KeyCode::M, L"m", KeyCode::M, L"M", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f);
			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::Comma, L",", 0.1f);
			SpaceKeyboardKey _keySpace = SpaceKeyboardKey();
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::Period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey( L"Enter", 0.2f);
	};

	class RussianKeyboardLayout : public KeyboardLayout {
		public:
			RussianKeyboardLayout();

		private:
			TextKeyboardKey _keyQ = TextKeyboardKey(KeyCode::Q, L"й", KeyCode::Q, L"Й", 1.f / 11.f);
			TextKeyboardKey _keyW = TextKeyboardKey(KeyCode::W, L"ц", KeyCode::W, L"Ц", 1.f / 11.f);
			TextKeyboardKey _keyE = TextKeyboardKey(KeyCode::E, L"у", KeyCode::E, L"У", 1.f / 11.f);
			TextKeyboardKey _keyR = TextKeyboardKey(KeyCode::R, L"к", KeyCode::R, L"К", 1.f / 11.f);
			TextKeyboardKey _keyT = TextKeyboardKey(KeyCode::T, L"е", KeyCode::T, L"Е", 1.f / 11.f);
			TextKeyboardKey _keyY = TextKeyboardKey(KeyCode::Y, L"н", KeyCode::Y, L"Н", 1.f / 11.f);
			TextKeyboardKey _keyU = TextKeyboardKey(KeyCode::U, L"г", KeyCode::U, L"Г", 1.f / 11.f);
			TextKeyboardKey _keyI = TextKeyboardKey(KeyCode::I, L"ш", KeyCode::I, L"Ш", 1.f / 11.f);
			TextKeyboardKey _keyO = TextKeyboardKey(KeyCode::O, L"щ", KeyCode::O, L"Щ", 1.f / 11.f);
			TextKeyboardKey _keyP = TextKeyboardKey(KeyCode::P, L"з", KeyCode::P, L"З", 1.f / 11.f);
			TextKeyboardKey _keyBracket = TextKeyboardKey(KeyCode::OpenBracket, L"х", KeyCode::OpenBracket, L"Х", 1.f / 11.f);

			TextKeyboardKey _keyA = TextKeyboardKey(KeyCode::A, L"ф", KeyCode::A, L"Ф", 1.f / 11.f);
			TextKeyboardKey _keyS = TextKeyboardKey(KeyCode::S, L"ы", KeyCode::S, L"Ы", 1.f / 11.f);
			TextKeyboardKey _keyD = TextKeyboardKey(KeyCode::D, L"в", KeyCode::D, L"В", 1.f / 11.f);
			TextKeyboardKey _keyF = TextKeyboardKey(KeyCode::F, L"а", KeyCode::F, L"А", 1.f / 11.f);
			TextKeyboardKey _keyG = TextKeyboardKey(KeyCode::G, L"п", KeyCode::G, L"П", 1.f / 11.f);
			TextKeyboardKey _keyH = TextKeyboardKey(KeyCode::H, L"р", KeyCode::H, L"Р", 1.f / 11.f);
			TextKeyboardKey _keyJ = TextKeyboardKey(KeyCode::J, L"о", KeyCode::J, L"О", 1.f / 11.f);
			TextKeyboardKey _keyK = TextKeyboardKey(KeyCode::K, L"л", KeyCode::K, L"Л", 1.f / 11.f);
			TextKeyboardKey _keyL = TextKeyboardKey(KeyCode::L, L"д", KeyCode::L, L"Д", 1.f / 11.f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(KeyCode::Semicolon, L"ж", KeyCode::Semicolon, L"Ж", 1.f / 11.f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(KeyCode::Quote, L"э", KeyCode::Quote, L"Э", 1.f / 11.f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 1.f / 11.f);
			TextKeyboardKey _keyZ = TextKeyboardKey(KeyCode::Z, L"я", KeyCode::Z, L"Я", 1.f / 11.f);
			TextKeyboardKey _keyX = TextKeyboardKey(KeyCode::X, L"ч", KeyCode::X, L"Ч", 1.f / 11.f);
			TextKeyboardKey _keyC = TextKeyboardKey(KeyCode::C, L"с", KeyCode::C, L"С", 1.f / 11.f);
			TextKeyboardKey _keyV = TextKeyboardKey(KeyCode::V, L"м", KeyCode::V, L"М", 1.f / 11.f);
			TextKeyboardKey _keyB = TextKeyboardKey(KeyCode::B, L"и", KeyCode::B, L"И", 1.f / 11.f);
			TextKeyboardKey _keyN = TextKeyboardKey(KeyCode::N, L"т", KeyCode::N, L"Т", 1.f / 11.f);
			TextKeyboardKey _keyM = TextKeyboardKey(KeyCode::M, L"ь", KeyCode::M, L"Ь", 1.f / 11.f);
			TextKeyboardKey _keyLessThan = TextKeyboardKey(KeyCode::LessThan, L"б", KeyCode::M, L"Б", 1.f / 11.f);
			TextKeyboardKey _keyMoreThan = TextKeyboardKey(KeyCode::MoreThan, L"ю", KeyCode::M, L"Ю", 1.f / 11.f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 1.f / 11.f);

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f);
			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::Comma, L",", 0.1f);
			SpaceKeyboardKey _keySpace = SpaceKeyboardKey();
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::Period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey( L"Enter", 0.2f);
	};
}