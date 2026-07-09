#pragma once

#include <vector>

#include <YOBA/UI/Keyboard/Button.hpp>
#include <YOBA/UI/Shapes/Rectangle.hpp>
#include <YOBA/UI/Layout.hpp>

#include <YOBA/UI/Keyboard/Button.hpp>
#include <YOBA/Core/Key.hpp>
#include <YOBA/UI/Layout.hpp>

namespace YOBA {
	class Keyboard;
	class KeyboardButton;

	class KeyboardLayout : public Layout {
		public:
			explicit KeyboardLayout(const std::vector<std::vector<KeyboardButton*>>& keys);

			void assignKeyboard(Keyboard* keyboard);
			const std::vector<std::vector<KeyboardButton*>>& getButtons() const;

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onArrange(const Rectangle& bounds) override;

		private:
			std::vector<std::vector<KeyboardButton*>> _buttons;
			Keyboard* _keyboard = nullptr;
	};

	class NumericKeyboardLayout : public KeyboardLayout {
		public:
			NumericKeyboardLayout();

		private:
			TextKeyboardButton _key1 { Key::number1, "1", 1.f / 4.f };
			TextKeyboardButton _key2 { Key::number2, "2", 1.f / 4.f };
			TextKeyboardButton _key3 { Key::number3, "3", 1.f / 4.f };
			DummyKeyboardButton _keyDummy30 { 1.f / 4.f };

			TextKeyboardButton _key4 { Key::number4, "4", 1.f / 4.f };
			TextKeyboardButton _key5 { Key::number5, "5", 1.f / 4.f };
			TextKeyboardButton _key6 { Key::number6, "6", 1.f / 4.f };
			DummyKeyboardButton _keyDummy31 { 1.f / 4.f };

			TextKeyboardButton _key7 { Key::number7, "7", 1.f / 4.f };
			TextKeyboardButton _key8 { Key::number8, "8", 1.f / 4.f };
			TextKeyboardButton _key9 { Key::number9, "9", 1.f / 4.f };
			BackspaceKeyboardButton _keyBackspace { "<", 1.f / 4.f };

			NumericSignMinusKeyboardKey _keyMinus { 1.f / 4.f };
			TextKeyboardButton _key0 { Key::number0, "0", Key::number0, "0", 1.f / 4.f };
			PeriodKeyboardButton _keyPeriod { 1.f / 4.f };
			EnterKeyboardButton _keyEnter { 1.f / 4.f };
	};

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardButton _key1 { Key::number1, "1", Key::deadTilde, "~", 0.1f };
			TextKeyboardButton _key2 { Key::number2, "2", Key::deadGrave, "`", 0.1f };
			TextKeyboardButton _key3 { Key::number3, "3", Key::verticalBar, "|", 0.1f };
			TextKeyboardButton _key4 { Key::number4, "4", Key::bullet, "•", 0.1f };
			TextKeyboardButton _key5 { Key::number5, "5", Key::squareRoot, "√", 0.1f };
			TextKeyboardButton _key6 { Key::number6, "6", Key::pi, "π", 0.1f };
			TextKeyboardButton _key7 { Key::number7, "7", Key::division, "÷", 0.1f };
			TextKeyboardButton _key8 { Key::number8, "8", Key::multiplication, "×", 0.1f };
			TextKeyboardButton _key9 { Key::number9, "9", Key::section, "§", 0.1f };
			TextKeyboardButton _key0 { Key::number0, "0", Key::delta, "Δ", 0.1f };

			TextKeyboardButton _keyAt { Key::at, "@", Key::pound, "£", 0.1f };
			TextKeyboardButton _keyNumberSign { Key::numberSign, "#", Key::cent, "¢", 0.1f };
			TextKeyboardButton _keyDollar { Key::dollar, "$", Key::euro, "€", 0.1f };
			TextKeyboardButton _keyUnderscore { Key::underscore, "_", Key::yen, "¥", 0.1f };
			TextKeyboardButton _keyAmpersand { Key::ampersand, "&", Key::circumflex, "^", 0.1f };
			TextKeyboardButton _keyMinus { Key::minus, "-", Key::degree, "°", 0.1f };
			TextKeyboardButton _keyPlus { Key::plus, "+", Key::equals, "=", 0.1f };
			TextKeyboardButton _keyLeftBrace { Key::leftBrace, "(", Key::leftParenthesis, "{", 0.1f };
			TextKeyboardButton _keyRightBrace { Key::rightBrace, ")", Key::rightParenthesis, "}", 0.1f };
			TextKeyboardButton _keySlash { Key::slash, "/", Key::backSlash, "\\", 0.1f };

			ShiftKeyboardButton _keyShift { "^", "^^", "^^^", 0.15f };
			TextKeyboardButton _keyAsterisk { Key::asterisk, "*", Key::percent, "%", 0.1f };
			TextKeyboardButton _keyDoubleQuote { Key::doubleQuote, "\"", Key::copyright, "©", 0.1f };
			TextKeyboardButton _keyQuote { Key::quote, "\'", Key::registered, "®", 0.1f };
			TextKeyboardButton _keyColon { Key::colon, ":", Key::trademark, "™", 0.1f };
			TextKeyboardButton _keySemicolon { Key::semicolon, ";", Key::check, "✓", 0.1f };
			TextKeyboardButton _keyExclamationMark { Key::exclamationMark, "!", Key::openBracket, "[", 0.1f };
			TextKeyboardButton _keyQuestionMark { Key::questionMark, "?", Key::closeBracket, "]", 0.1f };
			BackspaceKeyboardButton _keyBackspace { "<", 0.15f };

			CurrentCyclicLayoutKeyboardButton _keyCyclicLayout { "Abc", 0.1f };
			TextKeyboardButton _keyComma { Key::comma, ",", 0.1f };
			TextKeyboardButton _keySpace { Key::space, " ", 0.5f };
			PeriodKeyboardButton _keyPeriod { 0.1f };
			EnterKeyboardButton _keyEnter { 0.2f };
	};

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			TextKeyboardButton _keyQ { Key::q, "q", Key::q, "Q", 0.1f };
			TextKeyboardButton _keyW { Key::w, "w", Key::w, "W", 0.1f };
			TextKeyboardButton _keyE { Key::e, "e", Key::e, "E", 0.1f };
			TextKeyboardButton _keyR { Key::r, "r", Key::r, "R", 0.1f };
			TextKeyboardButton _keyT { Key::t, "t", Key::t, "T", 0.1f };
			TextKeyboardButton _keyY { Key::y, "y", Key::y, "Y", 0.1f };
			TextKeyboardButton _keyU { Key::u, "u", Key::u, "U", 0.1f };
			TextKeyboardButton _keyI { Key::i, "i", Key::i, "I", 0.1f };
			TextKeyboardButton _keyO { Key::o, "o", Key::o, "O", 0.1f };
			TextKeyboardButton _keyP { Key::p, "p", Key::p, "P", 0.1f };

			TextKeyboardButton _keyA { Key::a, "a", Key::a, "A", 0.1f };
			TextKeyboardButton _keyS { Key::s, "s", Key::s, "S", 0.1f };
			TextKeyboardButton _keyD { Key::d, "d", Key::d, "D", 0.1f };
			TextKeyboardButton _keyF { Key::f, "f", Key::f, "F", 0.1f };
			TextKeyboardButton _keyG { Key::g, "g", Key::g, "G", 0.1f };
			TextKeyboardButton _keyH { Key::h, "h", Key::h, "H", 0.1f };
			TextKeyboardButton _keyJ { Key::j, "j", Key::j, "J", 0.1f };
			TextKeyboardButton _keyK { Key::k, "k", Key::k, "K", 0.1f };
			TextKeyboardButton _keyL { Key::l, "l", Key::l, "L", 0.1f };

			ShiftKeyboardButton _keyShift = { "^", "^^", "^^^", 0.15f };
			TextKeyboardButton _keyZ { Key::z, "z", Key::z, "Z", 0.1f };
			TextKeyboardButton _keyX { Key::x, "x", Key::x, "X", 0.1f };
			TextKeyboardButton _keyC { Key::c, "c", Key::c, "C", 0.1f };
			TextKeyboardButton _keyV { Key::v, "v", Key::v, "V", 0.1f };
			TextKeyboardButton _keyB { Key::b, "b", Key::b, "B", 0.1f };
			TextKeyboardButton _keyN { Key::n, "n", Key::n, "N", 0.1f };
			TextKeyboardButton _keyM { Key::m, "m", Key::m, "M", 0.1f };
			BackspaceKeyboardButton _keyBackspace { "<", 0.15f };

			CharactersLayoutKeyboardButton _keyCharactersLayout { "123", 0.1f };
			CyclicLayoutKeyboardButton _keyCyclicLayout { "Lang", 0.1f };
			TextKeyboardButton _keyComma { Key::comma, ",", 0.1f };
			SpaceKeyboardButton _keySpace  {  };
			PeriodKeyboardButton _keyPeriod { 0.1f };
			EnterKeyboardButton _keyEnter { 0.2f };
	};

	class RussianKeyboardLayout : public KeyboardLayout {
		public:
			RussianKeyboardLayout();

		private:
			TextKeyboardButton _keyQ { Key::q, "й", Key::q, "Й", 1.f / 11.f };
			TextKeyboardButton _keyW { Key::w, "ц", Key::w, "Ц", 1.f / 11.f };
			TextKeyboardButton _keyE { Key::e, "у", Key::e, "У", 1.f / 11.f };
			TextKeyboardButton _keyR { Key::r, "к", Key::r, "К", 1.f / 11.f };
			TextKeyboardButton _keyT { Key::t, "е", Key::t, "Е", 1.f / 11.f };
			TextKeyboardButton _keyY { Key::y, "н", Key::y, "Н", 1.f / 11.f };
			TextKeyboardButton _keyU { Key::u, "г", Key::u, "Г", 1.f / 11.f };
			TextKeyboardButton _keyI { Key::i, "ш", Key::i, "Ш", 1.f / 11.f };
			TextKeyboardButton _keyO { Key::o, "щ", Key::o, "Щ", 1.f / 11.f };
			TextKeyboardButton _keyP { Key::p, "з", Key::p, "З", 1.f / 11.f };
			TextKeyboardButton _keyBracket { Key::openBracket, "х", Key::openBracket, "Х", 1.f / 11.f };

			TextKeyboardButton _keyA { Key::a, "ф", Key::a, "Ф", 1.f / 11.f };
			TextKeyboardButton _keyS { Key::s, "ы", Key::s, "Ы", 1.f / 11.f };
			TextKeyboardButton _keyD { Key::d, "в", Key::d, "В", 1.f / 11.f };
			TextKeyboardButton _keyF { Key::f, "а", Key::f, "А", 1.f / 11.f };
			TextKeyboardButton _keyG { Key::g, "п", Key::g, "П", 1.f / 11.f };
			TextKeyboardButton _keyH { Key::h, "р", Key::h, "Р", 1.f / 11.f };
			TextKeyboardButton _keyJ { Key::j, "о", Key::j, "О", 1.f / 11.f };
			TextKeyboardButton _keyK { Key::k, "л", Key::k, "Л", 1.f / 11.f };
			TextKeyboardButton _keyL { Key::l, "д", Key::l, "Д", 1.f / 11.f };
			TextKeyboardButton _keySemicolon { Key::semicolon, "ж", Key::semicolon, "Ж", 1.f / 11.f };
			TextKeyboardButton _keyQuote { Key::quote, "э", Key::quote, "Э", 1.f / 11.f };

			ShiftKeyboardButton _keyShift { "^", "^^", "^^^", 1.f / 11.f };
			TextKeyboardButton _keyZ { Key::z, "я", Key::z, "Я", 1.f / 11.f };
			TextKeyboardButton _keyX { Key::x, "ч", Key::x, "Ч", 1.f / 11.f };
			TextKeyboardButton _keyC { Key::c, "с", Key::c, "С", 1.f / 11.f };
			TextKeyboardButton _keyV { Key::v, "м", Key::v, "М", 1.f / 11.f };
			TextKeyboardButton _keyB { Key::b, "и", Key::b, "И", 1.f / 11.f };
			TextKeyboardButton _keyN { Key::n, "т", Key::n, "Т", 1.f / 11.f };
			TextKeyboardButton _keyM { Key::m, "ь", Key::m, "Ь", 1.f / 11.f };
			TextKeyboardButton _keyLessThan { Key::lessThan, "б", Key::m, "Б", 1.f / 11.f };
			TextKeyboardButton _keyMoreThan { Key::moreThan, "ю", Key::m, "Ю", 1.f / 11.f };
			BackspaceKeyboardButton _keyBackspace { "<", 1.f / 11.f };

			CharactersLayoutKeyboardButton _keyCharactersLayout { "123", 0.1f };
			CyclicLayoutKeyboardButton _keyCyclicLayout { "Lang", 0.1f };
			TextKeyboardButton _keyComma { Key::comma, ",", 0.1f };
			SpaceKeyboardButton _keySpace {  };
			PeriodKeyboardButton _keyPeriod { 0.1f };
			EnterKeyboardButton _keyEnter { 0.2f };
	};
}