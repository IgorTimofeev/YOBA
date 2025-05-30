#pragma once

#include <optional>
#include <cstdint>

#include <YOBA/main/key.h>
#include <YOBA/main/events/keyEvent.h>

#include <YOBA/UI/traits/fontElement.h>
#include <YOBA/UI/shapes/rectangle.h>
#include <YOBA/UI/layout.h>
#include <YOBA/UI/button.h>
#include <YOBA/UI/relativeStackLayout.h>

namespace YOBA {
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
				Key key,
				std::wstring_view name,
				float width
			);

			virtual ~KeyboardKey() = default;

			static float stretched;

			virtual void tick(KeyboardButton* button);
			virtual void onKeyPressedChanged(KeyboardButton* button);
			virtual Key getKeyFromCase(Keyboard* keyboard) const;
			virtual std::wstring_view getNameFromCase(Keyboard* keyboard) const;
			virtual std::optional<std::wstring_view> getTextFromCase(Keyboard* keyboard) const;

			float getWidth() const;
			KeyboardKeyType getType() const;

			Key getKey() const;
			std::wstring_view getName() const;

		private:
			KeyboardKeyType _type;
			Key _key;
			const std::wstring_view _name;
			float _width;
	};

	class TextKeyboardKey : public KeyboardKey {
		public:
			TextKeyboardKey(
				Key key,
				std::wstring_view name,
				Key uppercaseKey,
				std::wstring_view uppercaseName,
				float width
			);

			TextKeyboardKey(
				Key key,
				std::wstring_view name,
				float width
			);

			Key getUppercaseKey() const;
			std::wstring_view getUppercaseName() const;

			Key getKeyFromCase(Keyboard* keyboard) const override;

			std::wstring_view getNameFromCase(Keyboard* keyboard) const override;
			std::optional<std::wstring_view> getTextFromCase(Keyboard* keyboard) const override;

			void onKeyPressedChanged(KeyboardButton* button) override;

		private:
			Key _uppercaseKey;
			const std::wstring_view _uppercaseName;
	};

	class ShiftKeyboardKey : public KeyboardKey {
		public:
			ShiftKeyboardKey(std::wstring_view name, std::wstring_view uppercaseName, std::wstring_view capsName, float width);

			void onKeyPressedChanged(KeyboardButton* button) override;

			std::wstring_view getNameFromCase(Keyboard* keyboard) const override;

		private:
			const std::wstring_view _uppercaseName;
			const std::wstring_view _capsName;
	};

	class BackspaceKeyboardKey : public KeyboardKey {
		public:
			BackspaceKeyboardKey(std::wstring_view name, float width);
	};

	class EnterKeyboardKey : public KeyboardKey {
		public:
			explicit EnterKeyboardKey(float width);
	};

	class SpaceKeyboardKey : public TextKeyboardKey {
		public:
			SpaceKeyboardKey();
	};

	class CharactersLayoutKeyboardKey : public KeyboardKey {
		public:
			CharactersLayoutKeyboardKey(std::wstring_view name, float width);

			void onKeyPressedChanged(KeyboardButton* button) override;

		private:
			std::function<KeyboardLayout*()> _layoutBuilder;
	};

	class CurrentCyclicLayoutKeyboardKey : public KeyboardKey {
		public:
			CurrentCyclicLayoutKeyboardKey(std::wstring_view name, float width);

			void onKeyPressedChanged(KeyboardButton* button) override;
	};

	class CyclicLayoutKeyboardKey : public KeyboardKey {
		public:
			CyclicLayoutKeyboardKey(std::wstring_view name, float width);

			void onKeyPressedChanged(KeyboardButton* button) override;
	};

	class KeyboardButtonsLayout : public Layout {
		public:
			explicit KeyboardButtonsLayout(Keyboard* keyboard);

		protected:
			Size onMeasure(const Size& availableSize) override;

			void onRender(Renderer* renderer, const Bounds& bounds) override;

		private:
			Keyboard* _keyboard;
	};

	class KeyboardLayout {
		public:
			explicit KeyboardLayout(const std::vector<std::vector<KeyboardKey*>>& keys) : keys(keys) {

			}

			std::vector<std::vector<KeyboardKey*>> keys;
	};

	class KeyboardButton : public Button {
		public:
			KeyboardButton(Keyboard* keyboard, uint8_t row, uint8_t column);

			void onTick() override;

			Keyboard* getKeyboard() const;
			KeyboardKey* getKey() const;

			uint8_t getRow() const;
			uint8_t getColumn() const;

			void updateFromCase();

		protected:
			void onIsActiveChanged() override;

		private:
			Keyboard* _keyboard;
			uint8_t _row;
			uint8_t _column;

			void updateTextFromCase();
	};

	class Keyboard : public Layout, public FontElement {
		public:
			Keyboard();

			~Keyboard() override;

			void setLayout(KeyboardLayout* value);
			KeyboardLayout* getLayout() const;

			uint8_t getCyclicLayoutsCount() const;
			int8_t getCyclicLayoutIndex() const;
			void setCyclicLayoutIndex(int8_t value);

			void setNextCyclicLayoutIndex();

			const Color* getBackgroundColor() const;

			void setBackgroundColor(const Color* value);

			const Color* getDefaultKeyBackgroundColor() const;
			void setDefaultKeyBackgroundColor(const Color* value);

			const Color* getDefaultKeyTextColor() const;
			void setDefaultKeyTextColor(const Color* value);

			const Color* getActionKeyBackgroundColor() const;
			void setActionKeyBackgroundColor(const Color* value);

			const Color* getActionKeyTextColor() const;
			void setActionKeyTextColor(const Color* actionButtonSecondaryColor);

			uint8_t getHorizontalKeySpacing() const;
			void setHorizontalKeySpacing(uint8_t horizontalKeySpacing);

			uint8_t getVerticalKeySpacing() const;
			void setVerticalKeySpacing(uint8_t value);

			uint8_t getKeyCornerRadius() const;
			void setKeyCornerRadius(uint8_t value);

			uint16_t getKeyHeight() const;
			void setKeyHeight(uint16_t value);

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
			const Color* _defaultKeyPrimaryColor = nullptr;
			const Color* _defaultKeyTextColor = nullptr;
			const Color* _actionKeyBackgroundColor = nullptr;
			const Color* _actionKeyTextColor = nullptr;

			KeyboardCase _case = KeyboardCase::lower;

			uint8_t _keyCornerRadius = 0;
			uint32_t _continuousTypingDelay = 500'000;
			uint32_t _continuousTypingInterval = 50'000;

			int8_t _cyclicLayoutIndex = -1;
			KeyboardLayout* _layout = nullptr;
			std::vector<std::function<KeyboardLayout*()>> _cyclicLayoutBuilders;
			std::optional<std::function<KeyboardLayout*()>> _charactersLayoutBuilder = std::nullopt;

			Rectangle _backgroundPanel = Rectangle();
			KeyboardButtonsLayout _buttonsLayout;

			uint8_t _horizontalKeySpacing = 1;
			uint8_t _verticalKeySpacing = 1;
			uint16_t _keyHeight = 25;

			void deleteLayoutAndUIElements();
	};

	class KeyboardControllerLayout : public RelativeStackLayout {
		public:
			KeyboardControllerLayout();

			Keyboard keyboard {};
			Layout temporaryLayout {};
	};

	class KeyboardController {
		public:
			static void show();
			static void hide();
			static bool isVisible();

			static void setOnShow(const std::optional<std::function<void(Keyboard*)>>& value);
			static Keyboard* getKeyboard();

			static Layout* getTargetLayout();
			static void setTargetLayout(Layout* value);

		private:
			static std::optional<std::function<void(Keyboard*)>> _onKeyboardShow;
			static KeyboardControllerLayout* _controllerLayout;
			static Layout* _targetLayout;

			static Layout* getTargetLayoutOrApplication();
	};

	// ----------------------------- Layouts -----------------------------

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardKey _key1 = TextKeyboardKey(Key::number1, L"1", Key::deadTilde, L"~", 0.1f);
			TextKeyboardKey _key2 = TextKeyboardKey(Key::number2, L"2", Key::deadGrave, L"`", 0.1f);
			TextKeyboardKey _key3 = TextKeyboardKey(Key::number3, L"3", Key::verticalBar, L"|", 0.1f);
			TextKeyboardKey _key4 = TextKeyboardKey(Key::number4, L"4", Key::bullet, L"•", 0.1f);
			TextKeyboardKey _key5 = TextKeyboardKey(Key::number5, L"5", Key::squareRoot, L"√", 0.1f);
			TextKeyboardKey _key6 = TextKeyboardKey(Key::number6, L"6", Key::pi, L"π", 0.1f);
			TextKeyboardKey _key7 = TextKeyboardKey(Key::number7, L"7", Key::division, L"÷", 0.1f);
			TextKeyboardKey _key8 = TextKeyboardKey(Key::number8, L"8", Key::multiplication, L"×", 0.1f);
			TextKeyboardKey _key9 = TextKeyboardKey(Key::number9, L"9", Key::section, L"§", 0.1f);
			TextKeyboardKey _key0 = TextKeyboardKey(Key::number0, L"0", Key::delta, L"Δ", 0.1f);

			TextKeyboardKey _keyAt = TextKeyboardKey(Key::at, L"@", Key::pound, L"£", 0.1f);
			TextKeyboardKey _keyNumberSign = TextKeyboardKey(Key::numberSign, L"#", Key::cent, L"¢", 0.1f);
			TextKeyboardKey _keyDollar = TextKeyboardKey(Key::dollar, L"$", Key::euro, L"€", 0.1f);
			TextKeyboardKey _keyUnderscore = TextKeyboardKey(Key::underscore, L"_", Key::yen, L"¥", 0.1f);
			TextKeyboardKey _keyAmpersand = TextKeyboardKey(Key::ampersand, L"&", Key::circumflex, L"^", 0.1f);
			TextKeyboardKey _keyMinus = TextKeyboardKey(Key::minus, L"-", Key::degree, L"°", 0.1f);
			TextKeyboardKey _keyPlus = TextKeyboardKey(Key::plus, L"+", Key::equals, L"=", 0.1f);
			TextKeyboardKey _keyLeftBrace = TextKeyboardKey(Key::leftBrace, L"(", Key::leftParenthesis, L"{", 0.1f);
			TextKeyboardKey _keyRightBrace = TextKeyboardKey(Key::rightBrace, L")", Key::rightParenthesis, L"}", 0.1f);
			TextKeyboardKey _keySlash = TextKeyboardKey(Key::slash, L"/", Key::backSlash, L"\\", 0.1f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyAsterisk = TextKeyboardKey(Key::asterisk, L"*", Key::percent, L"%", 0.1f);
			TextKeyboardKey _keyDoubleQuote = TextKeyboardKey(Key::doubleQuote, L"\"", Key::copyright, L"©", 0.1f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(Key::quote, L"\'", Key::registered, L"®", 0.1f);
			TextKeyboardKey _keyColon = TextKeyboardKey(Key::colon, L":", Key::trademark, L"™", 0.1f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(Key::semicolon, L";", Key::check, L"✓", 0.1f);
			TextKeyboardKey _keyExclamationMark = TextKeyboardKey(Key::exclamationMark, L"!", Key::openBracket, L"[", 0.1f);
			TextKeyboardKey _keyQuestionMark = TextKeyboardKey(Key::questionMark, L"?", Key::closeBracket, L"]", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);

			CurrentCyclicLayoutKeyboardKey _keyCyclicLayout = CurrentCyclicLayoutKeyboardKey(L"Abc", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(Key::comma, L",", 0.1f);
			TextKeyboardKey _keySpace = TextKeyboardKey(Key::space, L" ", 0.5f);
			TextKeyboardKey _keyPeriod = TextKeyboardKey(Key::period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(0.2f);
	};

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			TextKeyboardKey _keyQ = TextKeyboardKey(Key::q, L"q", Key::q, L"Q", 0.1f);
			TextKeyboardKey _keyW = TextKeyboardKey(Key::w, L"w", Key::w, L"W", 0.1f);
			TextKeyboardKey _keyE = TextKeyboardKey(Key::e, L"e", Key::e, L"E", 0.1f);
			TextKeyboardKey _keyR = TextKeyboardKey(Key::r, L"r", Key::r, L"R", 0.1f);
			TextKeyboardKey _keyT = TextKeyboardKey(Key::t, L"t", Key::t, L"T", 0.1f);
			TextKeyboardKey _keyY = TextKeyboardKey(Key::y, L"y", Key::y, L"Y", 0.1f);
			TextKeyboardKey _keyU = TextKeyboardKey(Key::u, L"u", Key::u, L"L", 0.1f);
			TextKeyboardKey _keyI = TextKeyboardKey(Key::i, L"i", Key::i, L"I", 0.1f);
			TextKeyboardKey _keyO = TextKeyboardKey(Key::o, L"o", Key::o, L"O", 0.1f);
			TextKeyboardKey _keyP = TextKeyboardKey(Key::p, L"p", Key::p, L"P", 0.1f);

			TextKeyboardKey _keyA = TextKeyboardKey(Key::a, L"a", Key::a, L"A", 0.1f);
			TextKeyboardKey _keyS = TextKeyboardKey(Key::s, L"s", Key::s, L"S", 0.1f);
			TextKeyboardKey _keyD = TextKeyboardKey(Key::d, L"d", Key::d, L"D", 0.1f);
			TextKeyboardKey _keyF = TextKeyboardKey(Key::f, L"f", Key::f, L"F", 0.1f);
			TextKeyboardKey _keyG = TextKeyboardKey(Key::g, L"g", Key::g, L"G", 0.1f);
			TextKeyboardKey _keyH = TextKeyboardKey(Key::h, L"h", Key::h, L"H", 0.1f);
			TextKeyboardKey _keyJ = TextKeyboardKey(Key::j, L"j", Key::j, L"J", 0.1f);
			TextKeyboardKey _keyK = TextKeyboardKey(Key::k, L"k", Key::k, L"K", 0.1f);
			TextKeyboardKey _keyL = TextKeyboardKey(Key::l, L"l", Key::l, L"L", 0.1f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 0.15f);
			TextKeyboardKey _keyZ = TextKeyboardKey(Key::z, L"z", Key::z, L"Z", 0.1f);
			TextKeyboardKey _keyX = TextKeyboardKey(Key::x, L"x", Key::x, L"X", 0.1f);
			TextKeyboardKey _keyC = TextKeyboardKey(Key::c, L"c", Key::c, L"C", 0.1f);
			TextKeyboardKey _keyV = TextKeyboardKey(Key::v, L"v", Key::v, L"V", 0.1f);
			TextKeyboardKey _keyB = TextKeyboardKey(Key::b, L"b", Key::b, L"B", 0.1f);
			TextKeyboardKey _keyN = TextKeyboardKey(Key::n, L"n", Key::n, L"N", 0.1f);
			TextKeyboardKey _keyM = TextKeyboardKey(Key::m, L"m", Key::m, L"M", 0.1f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 0.15f);

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f);
			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(Key::comma, L",", 0.1f);
			SpaceKeyboardKey _keySpace = SpaceKeyboardKey();
			TextKeyboardKey _keyPeriod = TextKeyboardKey(Key::period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(0.2f);
	};

	class RussianKeyboardLayout : public KeyboardLayout {
		public:
			RussianKeyboardLayout();

		private:
			TextKeyboardKey _keyQ = TextKeyboardKey(Key::q, L"й", Key::q, L"Й", 1.f / 11.f);
			TextKeyboardKey _keyW = TextKeyboardKey(Key::w, L"ц", Key::w, L"Ц", 1.f / 11.f);
			TextKeyboardKey _keyE = TextKeyboardKey(Key::e, L"у", Key::e, L"У", 1.f / 11.f);
			TextKeyboardKey _keyR = TextKeyboardKey(Key::r, L"к", Key::r, L"К", 1.f / 11.f);
			TextKeyboardKey _keyT = TextKeyboardKey(Key::t, L"е", Key::t, L"Е", 1.f / 11.f);
			TextKeyboardKey _keyY = TextKeyboardKey(Key::y, L"н", Key::y, L"Н", 1.f / 11.f);
			TextKeyboardKey _keyU = TextKeyboardKey(Key::u, L"г", Key::u, L"Г", 1.f / 11.f);
			TextKeyboardKey _keyI = TextKeyboardKey(Key::i, L"ш", Key::i, L"Ш", 1.f / 11.f);
			TextKeyboardKey _keyO = TextKeyboardKey(Key::o, L"щ", Key::o, L"Щ", 1.f / 11.f);
			TextKeyboardKey _keyP = TextKeyboardKey(Key::p, L"з", Key::p, L"З", 1.f / 11.f);
			TextKeyboardKey _keyBracket = TextKeyboardKey(Key::openBracket, L"х", Key::openBracket, L"Х", 1.f / 11.f);

			TextKeyboardKey _keyA = TextKeyboardKey(Key::a, L"ф", Key::a, L"Ф", 1.f / 11.f);
			TextKeyboardKey _keyS = TextKeyboardKey(Key::s, L"ы", Key::s, L"Ы", 1.f / 11.f);
			TextKeyboardKey _keyD = TextKeyboardKey(Key::d, L"в", Key::d, L"В", 1.f / 11.f);
			TextKeyboardKey _keyF = TextKeyboardKey(Key::f, L"а", Key::f, L"А", 1.f / 11.f);
			TextKeyboardKey _keyG = TextKeyboardKey(Key::g, L"п", Key::g, L"П", 1.f / 11.f);
			TextKeyboardKey _keyH = TextKeyboardKey(Key::h, L"р", Key::h, L"Р", 1.f / 11.f);
			TextKeyboardKey _keyJ = TextKeyboardKey(Key::j, L"о", Key::j, L"О", 1.f / 11.f);
			TextKeyboardKey _keyK = TextKeyboardKey(Key::k, L"л", Key::k, L"Л", 1.f / 11.f);
			TextKeyboardKey _keyL = TextKeyboardKey(Key::l, L"д", Key::l, L"Д", 1.f / 11.f);
			TextKeyboardKey _keySemicolon = TextKeyboardKey(Key::semicolon, L"ж", Key::semicolon, L"Ж", 1.f / 11.f);
			TextKeyboardKey _keyQuote = TextKeyboardKey(Key::quote, L"э", Key::quote, L"Э", 1.f / 11.f);

			ShiftKeyboardKey _keyShift = ShiftKeyboardKey(L"^", L"^^", L"^^^", 1.f / 11.f);
			TextKeyboardKey _keyZ = TextKeyboardKey(Key::z, L"я", Key::z, L"Я", 1.f / 11.f);
			TextKeyboardKey _keyX = TextKeyboardKey(Key::x, L"ч", Key::x, L"Ч", 1.f / 11.f);
			TextKeyboardKey _keyC = TextKeyboardKey(Key::c, L"с", Key::c, L"С", 1.f / 11.f);
			TextKeyboardKey _keyV = TextKeyboardKey(Key::v, L"м", Key::v, L"М", 1.f / 11.f);
			TextKeyboardKey _keyB = TextKeyboardKey(Key::b, L"и", Key::b, L"И", 1.f / 11.f);
			TextKeyboardKey _keyN = TextKeyboardKey(Key::n, L"т", Key::n, L"Т", 1.f / 11.f);
			TextKeyboardKey _keyM = TextKeyboardKey(Key::m, L"ь", Key::m, L"Ь", 1.f / 11.f);
			TextKeyboardKey _keyLessThan = TextKeyboardKey(Key::lessThan, L"б", Key::m, L"Б", 1.f / 11.f);
			TextKeyboardKey _keyMoreThan = TextKeyboardKey(Key::moreThan, L"ю", Key::m, L"Ю", 1.f / 11.f);
			BackspaceKeyboardKey _keyBackspace = BackspaceKeyboardKey(L"<", 1.f / 11.f);

			CharactersLayoutKeyboardKey _keyCharactersLayout = CharactersLayoutKeyboardKey(L"123", 0.1f);
			CyclicLayoutKeyboardKey _keyCyclicLayout = CyclicLayoutKeyboardKey(L"Lang", 0.1f);
			TextKeyboardKey _keyComma = TextKeyboardKey(Key::comma, L",", 0.1f);
			SpaceKeyboardKey _keySpace = SpaceKeyboardKey();
			TextKeyboardKey _keyPeriod = TextKeyboardKey(Key::period, L".", 0.1f);
			EnterKeyboardKey _keyEnter = EnterKeyboardKey(0.2f);
	};
}