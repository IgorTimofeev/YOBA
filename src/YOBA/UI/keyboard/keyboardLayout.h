#pragma once

#include <vector>

#include <YOBA/UI/keyboard/button.h>
#include <YOBA/main/key.h>
#include <YOBA/UI/layout.h>

namespace YOBA {
	class Keyboard;
	class KeyboardButton;

	class KeyboardLayout {
		public:
			explicit KeyboardLayout(const std::vector<std::vector<KeyboardButton*>>& keys);

			std::vector<std::vector<KeyboardButton*>> keys;
	};

	class NumericKeyboardLayout : public KeyboardLayout {
		public:
			NumericKeyboardLayout();

		private:
			TextKeyboardButton _key1 { Key::number1, L"1", 1.f / 4.f };
			TextKeyboardButton _key2 { Key::number2, L"2", 1.f / 4.f };
			TextKeyboardButton _key3 { Key::number3, L"3", 1.f / 4.f };
			DummyKeyboardButton _keyDummy30 { 1.f / 4.f };

			TextKeyboardButton _key4 { Key::number4, L"4", 1.f / 4.f };
			TextKeyboardButton _key5 { Key::number5, L"5", 1.f / 4.f };
			TextKeyboardButton _key6 { Key::number6, L"6", 1.f / 4.f };
			DummyKeyboardButton _keyDummy31 { 1.f / 4.f };

			TextKeyboardButton _key7 { Key::number7, L"7", 1.f / 4.f };
			TextKeyboardButton _key8 { Key::number8, L"8", 1.f / 4.f };
			TextKeyboardButton _key9 { Key::number9, L"9", 1.f / 4.f };
			BackspaceKeyboardButton _keyBackspace { L"<", 1.f / 4.f };

			NumericSignMinusKeyboardKey _keyMinus { 1.f / 4.f };
			TextKeyboardButton _key0 { Key::number0, L"0", Key::number0, L"0", 1.f / 4.f };
			PeriodKeyboardButton _keyPeriod { 1.f / 4.f };
			EnterKeyboardButton _keyEnter { 1.f / 4.f };
	};

	class CharactersKeyboardLayout : public KeyboardLayout {
		public:
			CharactersKeyboardLayout();

		private:
			TextKeyboardButton _key1 { Key::number1, L"1", Key::deadTilde, L"~", 0.1f };
			TextKeyboardButton _key2 { Key::number2, L"2", Key::deadGrave, L"`", 0.1f };
			TextKeyboardButton _key3 { Key::number3, L"3", Key::verticalBar, L"|", 0.1f };
			TextKeyboardButton _key4 { Key::number4, L"4", Key::bullet, L"•", 0.1f };
			TextKeyboardButton _key5 { Key::number5, L"5", Key::squareRoot, L"√", 0.1f };
			TextKeyboardButton _key6 { Key::number6, L"6", Key::pi, L"π", 0.1f };
			TextKeyboardButton _key7 { Key::number7, L"7", Key::division, L"÷", 0.1f };
			TextKeyboardButton _key8 { Key::number8, L"8", Key::multiplication, L"×", 0.1f };
			TextKeyboardButton _key9 { Key::number9, L"9", Key::section, L"§", 0.1f };
			TextKeyboardButton _key0 { Key::number0, L"0", Key::delta, L"Δ", 0.1f };

			TextKeyboardButton _keyAt { Key::at, L"@", Key::pound, L"£", 0.1f };
			TextKeyboardButton _keyNumberSign { Key::numberSign, L"#", Key::cent, L"¢", 0.1f };
			TextKeyboardButton _keyDollar { Key::dollar, L"$", Key::euro, L"€", 0.1f };
			TextKeyboardButton _keyUnderscore { Key::underscore, L"_", Key::yen, L"¥", 0.1f };
			TextKeyboardButton _keyAmpersand { Key::ampersand, L"&", Key::circumflex, L"^", 0.1f };
			TextKeyboardButton _keyMinus { Key::minus, L"-", Key::degree, L"°", 0.1f };
			TextKeyboardButton _keyPlus { Key::plus, L"+", Key::equals, L"=", 0.1f };
			TextKeyboardButton _keyLeftBrace { Key::leftBrace, L"(", Key::leftParenthesis, L"{", 0.1f };
			TextKeyboardButton _keyRightBrace { Key::rightBrace, L")", Key::rightParenthesis, L"}", 0.1f };
			TextKeyboardButton _keySlash { Key::slash, L"/", Key::backSlash, L"\\", 0.1f };

			ShiftKeyboardButton _keyShift { L"^", L"^^", L"^^^", 0.15f };
			TextKeyboardButton _keyAsterisk { Key::asterisk, L"*", Key::percent, L"%", 0.1f };
			TextKeyboardButton _keyDoubleQuote { Key::doubleQuote, L"\"", Key::copyright, L"©", 0.1f };
			TextKeyboardButton _keyQuote { Key::quote, L"\'", Key::registered, L"®", 0.1f };
			TextKeyboardButton _keyColon { Key::colon, L":", Key::trademark, L"™", 0.1f };
			TextKeyboardButton _keySemicolon { Key::semicolon, L";", Key::check, L"✓", 0.1f };
			TextKeyboardButton _keyExclamationMark { Key::exclamationMark, L"!", Key::openBracket, L"[", 0.1f };
			TextKeyboardButton _keyQuestionMark { Key::questionMark, L"?", Key::closeBracket, L"]", 0.1f };
			BackspaceKeyboardButton _keyBackspace { L"<", 0.15f };

			CurrentCyclicLayoutKeyboardButton _keyCyclicLayout { L"Abc", 0.1f };
			TextKeyboardButton _keyComma { Key::comma, L",", 0.1f };
			TextKeyboardButton _keySpace { Key::space, L" ", 0.5f };
			PeriodKeyboardButton _keyPeriod { 0.1f };
			EnterKeyboardButton _keyEnter { 0.2f };
	};

	class EnglishKeyboardLayout : public KeyboardLayout {
		public:
			EnglishKeyboardLayout();

		private:
			TextKeyboardButton _keyQ { Key::q, L"q", Key::q, L"Q", 0.1f };
			TextKeyboardButton _keyW { Key::w, L"w", Key::w, L"W", 0.1f };
			TextKeyboardButton _keyE { Key::e, L"e", Key::e, L"E", 0.1f };
			TextKeyboardButton _keyR { Key::r, L"r", Key::r, L"R", 0.1f };
			TextKeyboardButton _keyT { Key::t, L"t", Key::t, L"T", 0.1f };
			TextKeyboardButton _keyY { Key::y, L"y", Key::y, L"Y", 0.1f };
			TextKeyboardButton _keyU { Key::u, L"u", Key::u, L"L", 0.1f };
			TextKeyboardButton _keyI { Key::i, L"i", Key::i, L"I", 0.1f };
			TextKeyboardButton _keyO { Key::o, L"o", Key::o, L"O", 0.1f };
			TextKeyboardButton _keyP { Key::p, L"p", Key::p, L"P", 0.1f };

			TextKeyboardButton _keyA { Key::a, L"a", Key::a, L"A", 0.1f };
			TextKeyboardButton _keyS { Key::s, L"s", Key::s, L"S", 0.1f };
			TextKeyboardButton _keyD { Key::d, L"d", Key::d, L"D", 0.1f };
			TextKeyboardButton _keyF { Key::f, L"f", Key::f, L"F", 0.1f };
			TextKeyboardButton _keyG { Key::g, L"g", Key::g, L"G", 0.1f };
			TextKeyboardButton _keyH { Key::h, L"h", Key::h, L"H", 0.1f };
			TextKeyboardButton _keyJ { Key::j, L"j", Key::j, L"J", 0.1f };
			TextKeyboardButton _keyK { Key::k, L"k", Key::k, L"K", 0.1f };
			TextKeyboardButton _keyL { Key::l, L"l", Key::l, L"L", 0.1f };

			ShiftKeyboardButton _keyShift = { L"^", L"^^", L"^^^", 0.15f };
			TextKeyboardButton _keyZ { Key::z, L"z", Key::z, L"Z", 0.1f };
			TextKeyboardButton _keyX { Key::x, L"x", Key::x, L"X", 0.1f };
			TextKeyboardButton _keyC { Key::c, L"c", Key::c, L"C", 0.1f };
			TextKeyboardButton _keyV { Key::v, L"v", Key::v, L"V", 0.1f };
			TextKeyboardButton _keyB { Key::b, L"b", Key::b, L"B", 0.1f };
			TextKeyboardButton _keyN { Key::n, L"n", Key::n, L"N", 0.1f };
			TextKeyboardButton _keyM { Key::m, L"m", Key::m, L"M", 0.1f };
			BackspaceKeyboardButton _keyBackspace { L"<", 0.15f };

			CharactersLayoutKeyboardButton _keyCharactersLayout { L"123", 0.1f };
			CyclicLayoutKeyboardButton _keyCyclicLayout { L"Lang", 0.1f };
			TextKeyboardButton _keyComma { Key::comma, L",", 0.1f };
			SpaceKeyboardButton _keySpace  {  };
			PeriodKeyboardButton _keyPeriod { 0.1f };
			EnterKeyboardButton _keyEnter { 0.2f };
	};

	class RussianKeyboardLayout : public KeyboardLayout {
		public:
			RussianKeyboardLayout();

		private:
			TextKeyboardButton _keyQ { Key::q, L"й", Key::q, L"Й", 1.f / 11.f };
			TextKeyboardButton _keyW { Key::w, L"ц", Key::w, L"Ц", 1.f / 11.f };
			TextKeyboardButton _keyE { Key::e, L"у", Key::e, L"У", 1.f / 11.f };
			TextKeyboardButton _keyR { Key::r, L"к", Key::r, L"К", 1.f / 11.f };
			TextKeyboardButton _keyT { Key::t, L"е", Key::t, L"Е", 1.f / 11.f };
			TextKeyboardButton _keyY { Key::y, L"н", Key::y, L"Н", 1.f / 11.f };
			TextKeyboardButton _keyU { Key::u, L"г", Key::u, L"Г", 1.f / 11.f };
			TextKeyboardButton _keyI { Key::i, L"ш", Key::i, L"Ш", 1.f / 11.f };
			TextKeyboardButton _keyO { Key::o, L"щ", Key::o, L"Щ", 1.f / 11.f };
			TextKeyboardButton _keyP { Key::p, L"з", Key::p, L"З", 1.f / 11.f };
			TextKeyboardButton _keyBracket { Key::openBracket, L"х", Key::openBracket, L"Х", 1.f / 11.f };

			TextKeyboardButton _keyA { Key::a, L"ф", Key::a, L"Ф", 1.f / 11.f };
			TextKeyboardButton _keyS { Key::s, L"ы", Key::s, L"Ы", 1.f / 11.f };
			TextKeyboardButton _keyD { Key::d, L"в", Key::d, L"В", 1.f / 11.f };
			TextKeyboardButton _keyF { Key::f, L"а", Key::f, L"А", 1.f / 11.f };
			TextKeyboardButton _keyG { Key::g, L"п", Key::g, L"П", 1.f / 11.f };
			TextKeyboardButton _keyH { Key::h, L"р", Key::h, L"Р", 1.f / 11.f };
			TextKeyboardButton _keyJ { Key::j, L"о", Key::j, L"О", 1.f / 11.f };
			TextKeyboardButton _keyK { Key::k, L"л", Key::k, L"Л", 1.f / 11.f };
			TextKeyboardButton _keyL { Key::l, L"д", Key::l, L"Д", 1.f / 11.f };
			TextKeyboardButton _keySemicolon { Key::semicolon, L"ж", Key::semicolon, L"Ж", 1.f / 11.f };
			TextKeyboardButton _keyQuote { Key::quote, L"э", Key::quote, L"Э", 1.f / 11.f };

			ShiftKeyboardButton _keyShift { L"^", L"^^", L"^^^", 1.f / 11.f };
			TextKeyboardButton _keyZ { Key::z, L"я", Key::z, L"Я", 1.f / 11.f };
			TextKeyboardButton _keyX { Key::x, L"ч", Key::x, L"Ч", 1.f / 11.f };
			TextKeyboardButton _keyC { Key::c, L"с", Key::c, L"С", 1.f / 11.f };
			TextKeyboardButton _keyV { Key::v, L"м", Key::v, L"М", 1.f / 11.f };
			TextKeyboardButton _keyB { Key::b, L"и", Key::b, L"И", 1.f / 11.f };
			TextKeyboardButton _keyN { Key::n, L"т", Key::n, L"Т", 1.f / 11.f };
			TextKeyboardButton _keyM { Key::m, L"ь", Key::m, L"Ь", 1.f / 11.f };
			TextKeyboardButton _keyLessThan { Key::lessThan, L"б", Key::m, L"Б", 1.f / 11.f };
			TextKeyboardButton _keyMoreThan { Key::moreThan, L"ю", Key::m, L"Ю", 1.f / 11.f };
			BackspaceKeyboardButton _keyBackspace { L"<", 1.f / 11.f };

			CharactersLayoutKeyboardButton _keyCharactersLayout { L"123", 0.1f };
			CyclicLayoutKeyboardButton _keyCyclicLayout { L"Lang", 0.1f };
			TextKeyboardButton _keyComma { Key::comma, L",", 0.1f };
			SpaceKeyboardButton _keySpace {  };
			PeriodKeyboardButton _keyPeriod { 0.1f };
			EnterKeyboardButton _keyEnter { 0.2f };
	};
}