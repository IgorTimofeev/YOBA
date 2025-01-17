#pragma once

#include <optional>
#include <cstdint>
#include "layout.h"
#include "ui/traits/fontElement.h"
#include "button.h"
#include "ui/shapes/rectangle.h"
#include "equalStackLayout.h"

namespace yoba::ui {
	enum class KeyCode : uint16_t {
		none,
		cancel,
		backspace,
		tab,
		enter,
		clear,
		shift,
		ctrl,
		alt,
		pause,
		capsLock,
		kana,
		final,
		kanji,
		escape,
		convert,
		noConvert,
		accept,
		modeChange,
		space,
		pageUp,
		pageDown,
		end,
		home,
		left,
		up,
		right,
		down,
		comma,
		minus,
		period,
		slash,
		number0,
		number1,
		number2,
		number3,
		number4,
		number5,
		number6,
		number7,
		number8,
		number9,
		semicolon,
		equals,
		questionMark,
		a,
		b,
		c,
		d,
		e,
		f,
		g,
		h,
		i,
		j,
		k,
		l,
		m,
		n,
		o,
		p,
		q,
		r,
		s,
		t,
		u,
		v,
		w,
		x,
		y,
		z,
		openBracket,
		backSlash,
		closeBracket,
		percent,
		copyright,
		registered,
		trademark,
		check,
		numPad0,
		numPad1,
		numPad2,
		numPad3,
		numPad4,
		numPad5,
		numPad6,
		numPad7,
		numPad8,
		numPad9,
		numPadAsterisk,
		numPadPlus,
		numPadComma,
		numPadMinusf,
		numPadPeriod,
		numPadSlash,
		f1,
		f2,
		f3,
		f4,
		f5,
		f6,
		f7,
		f8,
		f9,
		f10,
		f11,
		f12,
		del,
		verticalBar,
		bullet,
		squareRoot,
		pi,
		division,
		multiplication,
		section,
		delta,
		pound,
		cent,
		yen,
		degree,
		deadGrave,
		deadAcute,
		deadCircumflex,
		deadTilde,
		deadMacron,
		deadBreve,
		deadAboveDot,
		deadDiaeresis,
		deadAboveRing,
		deadDoubleAcute,
		deadCaron,
		deadCedilla,
		deadOgonek,
		deadIota,
		deadVoicedSound,
		deadSemivoicedSound,
		numLock,
		scrollLock,
		ampersand,
		asterisk,
		doubleQuote,
		lessThan,
		moreThan,
		printScreen,
		insert,
		help,
		meta,
		greater,
		leftBrace,
		rightBrace,
		backQuote,
		quote,
		alphanumeric,
		katakana,
		hiragana,
		fullWidth,
		halfWidth,
		romanCharacters,
		allCandidates,
		previousCandidate,
		codeInput,
		japaneseKatakana,
		japaneseHiragana,
		japaneseRoman,
		kanaLock,
		inputMethodOnOff,
		at,
		colon,
		circumflex,
		dollar,
		euro,
		exclamationMark,
		invertedExclamationMark,
		leftParenthesis,
		numberSign,
		plus,
		rightParenthesis,
		underscore,
		windows,
		contextMenu
	};

	class Keyboard;
	class KeyboardLayout;
	class KeyboardButton;

	enum class KeyboardKeyType : uint8_t {
		normal,
		charactersLayout,
		cyclicLayout,
		action
	};

	enum class KeyboardCase : uint8_t {
		lower,
		upper,
		caps
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
			EnterKeyboardKey(float width);
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

	class KeyboardButtonsLayout : public Layout {
		public:
			explicit KeyboardButtonsLayout(Keyboard* keyboard);

		protected:
			Size onMeasure(Renderer* renderer, const Size& availableSize) override;

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

			void onTick() override;

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

	class Keyboard : public Layout, public FontElement {
		public:
			Keyboard();
		
			~Keyboard();

			Callback<KeyCode, bool> keyPressedChanged {};
			Callback<KeyCode, const std::wstring_view&> input {};

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

			uint16_t getContinuousTypingDelay() const;
			void setContinuousTypingDelay(uint16_t value);

			uint16_t getContinuousTypingInterval() const;
			void setContinuousTypingInterval(uint16_t value);

		private:
			const Color* _defaultButtonPrimaryColor = nullptr;
			const Color* _defaultButtonSecondaryColor = nullptr;
			const Color* _actionButtonPrimaryColor = nullptr;
			const Color* _actionButtonSecondaryColor = nullptr;

			KeyboardCase _case = KeyboardCase::lower;

			uint16_t _continuousTypingDelay = 500;
			uint16_t _continuousTypingInterval = 50;

			int8_t _cyclicLayoutIndex = -1;
			KeyboardLayout* _layout = nullptr;
			std::vector<std::function<KeyboardLayout*()>> _cyclicLayoutBuilders;
			std::optional<std::function<KeyboardLayout*()>> _charactersLayoutBuilder = std::nullopt;

			Rectangle _backgroundPanel = Rectangle();
			KeyboardButtonsLayout _buttonsLayout;

			uint8_t _horizontalKeySpacing = 2;
			uint8_t _verticalKeySpacing = 2;
			float _keyHeight = 0.1f;

			void deleteLayoutAndUIElements();
	};

	class KeyboardApplicationLayout : public Layout {
		protected:
			Size onMeasure(Renderer* renderer, const Size& availableSize) override;

			void onArrange(const Bounds& bounds) override;
	};

	class ApplicationKeyboardController {
		public:
			static Keyboard* show(Application* application);
			static void hide();

		private:
			static Keyboard* _keyboard;
			static Layout* _applicationChildrenLayout;
			static EqualStackLayout* _keyboardAndApplicationChildrenLayout;
	};

	// ----------------------------- Layouts -----------------------------

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardKey _key1 = TextKeyboardKey(KeyCode::number1, L"1", KeyCode::deadTilde, L"~", 0.1f);
			TextKeyboardKey _key2 = TextKeyboardKey(KeyCode::number2, L"2", KeyCode::deadGrave, L"`", 0.1f);
			TextKeyboardKey _key3 = TextKeyboardKey(KeyCode::number3, L"3", KeyCode::verticalBar, L"|", 0.1f);
			TextKeyboardKey _key4 = TextKeyboardKey(KeyCode::number4, L"4", KeyCode::bullet, L"•", 0.1f);
			TextKeyboardKey _key5 = TextKeyboardKey(KeyCode::number5, L"5", KeyCode::squareRoot, L"√", 0.1f);
			TextKeyboardKey _key6 = TextKeyboardKey(KeyCode::number6, L"6", KeyCode::pi, L"π", 0.1f);
			TextKeyboardKey _key7 = TextKeyboardKey(KeyCode::number7, L"7", KeyCode::division, L"÷", 0.1f);
			TextKeyboardKey _key8 = TextKeyboardKey(KeyCode::number8, L"8", KeyCode::multiplication, L"×", 0.1f);
			TextKeyboardKey _key9 = TextKeyboardKey(KeyCode::number9, L"9", KeyCode::section, L"§", 0.1f);
			TextKeyboardKey _key0 = TextKeyboardKey(KeyCode::number0, L"0", KeyCode::delta, L"Δ", 0.1f);

			TextKeyboardKey _keyAt = TextKeyboardKey(KeyCode::at, L"@", KeyCode::pound, L"£", 0.1f);
			TextKeyboardKey _keyNumberSign = TextKeyboardKey(KeyCode::numberSign, L"#", KeyCode::cent, L"¢", 0.1f);
			TextKeyboardKey _keyDollar = TextKeyboardKey(KeyCode::dollar, L"$", KeyCode::euro, L"€", 0.1f);
			TextKeyboardKey _keyUnderscore = TextKeyboardKey(KeyCode::underscore, L"_", KeyCode::yen, L"¥", 0.1f);
			TextKeyboardKey _keyAmpersand = TextKeyboardKey(KeyCode::ampersand, L"&", KeyCode::circumflex, L"^", 0.1f);
			TextKeyboardKey _keyMinus = TextKeyboardKey(KeyCode::minus, L"-", KeyCode::degree, L"°", 0.1f);
			TextKeyboardKey _keyPlus = TextKeyboardKey(KeyCode::plus, L"+", KeyCode::equals, L"=", 0.1f);
			TextKeyboardKey _keyLeftBrace = TextKeyboardKey(KeyCode::leftBrace, L"(", KeyCode::leftParenthesis, L"{", 0.1f);
			TextKeyboardKey _keyRightBrace = TextKeyboardKey(KeyCode::rightBrace, L")", KeyCode::rightParenthesis, L"}", 0.1f);
			TextKeyboardKey _keySlash = TextKeyboardKey(KeyCode::slash, L"/", KeyCode::backSlash, L"\\", 0.1f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyAsterisk = TextKeyboardKey(KeyCode::asterisk, L"*", KeyCode::percent, L"%", 0.1f);
			TextKeyboardKey _keyDoubleQuote = TextKeyboardKey(KeyCode::doubleQuote, L"\"", KeyCode::copyright, L"©", 0.1f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(KeyCode::quote, L"\'", KeyCode::registered, L"®", 0.1f);
			TextKeyboardKey _keyColon = TextKeyboardKey(KeyCode::colon, L":", KeyCode::trademark, L"™", 0.1f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(KeyCode::semicolon, L";", KeyCode::check, L"✓", 0.1f);
			TextKeyboardKey _keyExclamationMark = TextKeyboardKey(KeyCode::exclamationMark, L"!", KeyCode::openBracket, L"[", 0.1f);
			TextKeyboardKey _keyQuestionMark = TextKeyboardKey(KeyCode::questionMark, L"?", KeyCode::closeBracket, L"]", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);

			CurrentCyclicLayoutKeyboardKey _keyCyclicLayout = CurrentCyclicLayoutKeyboardKey(L"Abc", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::comma, L",", 0.1f);
			TextKeyboardKey _keySpace = TextKeyboardKey(KeyCode::space, L" ", 0.5f);
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(0.2f);
	};

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			TextKeyboardKey _keyQ = TextKeyboardKey(KeyCode::q, L"q", KeyCode::q, L"Q", 0.1f);
			TextKeyboardKey _keyW = TextKeyboardKey(KeyCode::w, L"w", KeyCode::w, L"W", 0.1f);
			TextKeyboardKey _keyE = TextKeyboardKey(KeyCode::e, L"e", KeyCode::e, L"E", 0.1f);
			TextKeyboardKey _keyR = TextKeyboardKey(KeyCode::r, L"r", KeyCode::r, L"R", 0.1f);
			TextKeyboardKey _keyT = TextKeyboardKey(KeyCode::t, L"t", KeyCode::t, L"T", 0.1f);
			TextKeyboardKey _keyY = TextKeyboardKey(KeyCode::y, L"y", KeyCode::y, L"Y", 0.1f);
			TextKeyboardKey _keyU = TextKeyboardKey(KeyCode::u, L"u", KeyCode::u, L"L", 0.1f);
			TextKeyboardKey _keyI = TextKeyboardKey(KeyCode::i, L"i", KeyCode::i, L"I", 0.1f);
			TextKeyboardKey _keyO = TextKeyboardKey(KeyCode::o, L"o", KeyCode::o, L"O", 0.1f);
			TextKeyboardKey _keyP = TextKeyboardKey(KeyCode::p, L"p", KeyCode::p, L"P", 0.1f);

			TextKeyboardKey _keyA = TextKeyboardKey(KeyCode::a, L"a", KeyCode::a, L"A", 0.1f);
			TextKeyboardKey _keyS = TextKeyboardKey(KeyCode::s, L"s", KeyCode::s, L"S", 0.1f);
			TextKeyboardKey _keyD = TextKeyboardKey(KeyCode::d, L"d", KeyCode::d, L"D", 0.1f);
			TextKeyboardKey _keyF = TextKeyboardKey(KeyCode::f, L"f", KeyCode::f, L"F", 0.1f);
			TextKeyboardKey _keyG = TextKeyboardKey(KeyCode::g, L"g", KeyCode::g, L"G", 0.1f);
			TextKeyboardKey _keyH = TextKeyboardKey(KeyCode::h, L"h", KeyCode::h, L"H", 0.1f);
			TextKeyboardKey _keyJ = TextKeyboardKey(KeyCode::j, L"j", KeyCode::j, L"J", 0.1f);
			TextKeyboardKey _keyK = TextKeyboardKey(KeyCode::k, L"k", KeyCode::k, L"K", 0.1f);
			TextKeyboardKey _keyL = TextKeyboardKey(KeyCode::l, L"l", KeyCode::l, L"L", 0.1f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyZ = TextKeyboardKey(KeyCode::z, L"z", KeyCode::z, L"Z", 0.1f);
			TextKeyboardKey _keyX = TextKeyboardKey(KeyCode::x, L"x", KeyCode::x, L"X", 0.1f);
			TextKeyboardKey _keyC = TextKeyboardKey(KeyCode::c, L"c", KeyCode::c, L"C", 0.1f);
			TextKeyboardKey _keyV = TextKeyboardKey(KeyCode::v, L"v", KeyCode::v, L"V", 0.1f);
			TextKeyboardKey _keyB = TextKeyboardKey(KeyCode::b, L"b", KeyCode::b, L"B", 0.1f);
			TextKeyboardKey _keyN = TextKeyboardKey(KeyCode::n, L"n", KeyCode::n, L"N", 0.1f);
			TextKeyboardKey _keyM = TextKeyboardKey(KeyCode::m, L"m", KeyCode::m, L"M", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f);
			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::comma, L",", 0.1f);
			SpaceKeyboardKey _keySpace = SpaceKeyboardKey();
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(0.2f);
	};

	class RussianKeyboardLayout : public KeyboardLayout {
		public:
			RussianKeyboardLayout();

		private:
			TextKeyboardKey _keyQ = TextKeyboardKey(KeyCode::q, L"й", KeyCode::q, L"Й", 1.f / 11.f);
			TextKeyboardKey _keyW = TextKeyboardKey(KeyCode::w, L"ц", KeyCode::w, L"Ц", 1.f / 11.f);
			TextKeyboardKey _keyE = TextKeyboardKey(KeyCode::e, L"у", KeyCode::e, L"У", 1.f / 11.f);
			TextKeyboardKey _keyR = TextKeyboardKey(KeyCode::r, L"к", KeyCode::r, L"К", 1.f / 11.f);
			TextKeyboardKey _keyT = TextKeyboardKey(KeyCode::t, L"е", KeyCode::t, L"Е", 1.f / 11.f);
			TextKeyboardKey _keyY = TextKeyboardKey(KeyCode::y, L"н", KeyCode::y, L"Н", 1.f / 11.f);
			TextKeyboardKey _keyU = TextKeyboardKey(KeyCode::u, L"г", KeyCode::u, L"Г", 1.f / 11.f);
			TextKeyboardKey _keyI = TextKeyboardKey(KeyCode::i, L"ш", KeyCode::i, L"Ш", 1.f / 11.f);
			TextKeyboardKey _keyO = TextKeyboardKey(KeyCode::o, L"щ", KeyCode::o, L"Щ", 1.f / 11.f);
			TextKeyboardKey _keyP = TextKeyboardKey(KeyCode::p, L"з", KeyCode::p, L"З", 1.f / 11.f);
			TextKeyboardKey _keyBracket = TextKeyboardKey(KeyCode::openBracket, L"х", KeyCode::openBracket, L"Х", 1.f / 11.f);

			TextKeyboardKey _keyA = TextKeyboardKey(KeyCode::a, L"ф", KeyCode::a, L"Ф", 1.f / 11.f);
			TextKeyboardKey _keyS = TextKeyboardKey(KeyCode::s, L"ы", KeyCode::s, L"Ы", 1.f / 11.f);
			TextKeyboardKey _keyD = TextKeyboardKey(KeyCode::d, L"в", KeyCode::d, L"В", 1.f / 11.f);
			TextKeyboardKey _keyF = TextKeyboardKey(KeyCode::f, L"а", KeyCode::f, L"А", 1.f / 11.f);
			TextKeyboardKey _keyG = TextKeyboardKey(KeyCode::g, L"п", KeyCode::g, L"П", 1.f / 11.f);
			TextKeyboardKey _keyH = TextKeyboardKey(KeyCode::h, L"р", KeyCode::h, L"Р", 1.f / 11.f);
			TextKeyboardKey _keyJ = TextKeyboardKey(KeyCode::j, L"о", KeyCode::j, L"О", 1.f / 11.f);
			TextKeyboardKey _keyK = TextKeyboardKey(KeyCode::k, L"л", KeyCode::k, L"Л", 1.f / 11.f);
			TextKeyboardKey _keyL = TextKeyboardKey(KeyCode::l, L"д", KeyCode::l, L"Д", 1.f / 11.f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(KeyCode::semicolon, L"ж", KeyCode::semicolon, L"Ж", 1.f / 11.f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(KeyCode::quote, L"э", KeyCode::quote, L"Э", 1.f / 11.f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 1.f / 11.f);
			TextKeyboardKey _keyZ = TextKeyboardKey(KeyCode::z, L"я", KeyCode::z, L"Я", 1.f / 11.f);
			TextKeyboardKey _keyX = TextKeyboardKey(KeyCode::x, L"ч", KeyCode::x, L"Ч", 1.f / 11.f);
			TextKeyboardKey _keyC = TextKeyboardKey(KeyCode::c, L"с", KeyCode::c, L"С", 1.f / 11.f);
			TextKeyboardKey _keyV = TextKeyboardKey(KeyCode::v, L"м", KeyCode::v, L"М", 1.f / 11.f);
			TextKeyboardKey _keyB = TextKeyboardKey(KeyCode::b, L"и", KeyCode::b, L"И", 1.f / 11.f);
			TextKeyboardKey _keyN = TextKeyboardKey(KeyCode::n, L"т", KeyCode::n, L"Т", 1.f / 11.f);
			TextKeyboardKey _keyM = TextKeyboardKey(KeyCode::m, L"ь", KeyCode::m, L"Ь", 1.f / 11.f);
			TextKeyboardKey _keyLessThan = TextKeyboardKey(KeyCode::lessThan, L"б", KeyCode::m, L"Б", 1.f / 11.f);
			TextKeyboardKey _keyMoreThan = TextKeyboardKey(KeyCode::moreThan, L"ю", KeyCode::m, L"Ю", 1.f / 11.f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 1.f / 11.f);

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f);
			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(KeyCode::comma, L",", 0.1f);
			SpaceKeyboardKey _keySpace = SpaceKeyboardKey();
			TextKeyboardKey _keyPeriod = TextKeyboardKey(KeyCode::period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(0.2f);
	};
}