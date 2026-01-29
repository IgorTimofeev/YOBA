#include <cstdint>

#include <YOBA/UI/keyboard/keyboardLayout.h>

namespace YOBA {
	KeyboardLayout::KeyboardLayout(const std::vector<std::vector<KeyboardButton*>>& keys) : keys(keys) {

	}

	// ----------------------------- NumericKeyboardLayout -----------------------------

	NumericKeyboardLayout::NumericKeyboardLayout() : KeyboardLayout({
		{
			&_key1,
			&_key2,
			&_key3,
			&_keyDummy30
		},
		{
			&_key4,
			&_key5,
			&_key6,
			&_keyDummy31
		},
		{
			&_key7,
			&_key8,
			&_key9,
			&_keyBackspace
		},

		{
			&_keyMinus,
			&_key0,
			&_keyPeriod,
			&_keyEnter
		},
	}) {

	}

	// ----------------------------- CharactersKeyboardLayout -----------------------------


	CharactersKeyboardLayout::CharactersKeyboardLayout() : KeyboardLayout({
		{
			&_key1,
			&_key2,
			&_key3,
			&_key4,
			&_key5,
			&_key6,
			&_key7,
			&_key8,
			&_key9,
			&_key0
		},
		{
			&_keyAt,
			&_keyNumberSign,
			&_keyDollar,
			&_keyUnderscore,
			&_keyAmpersand,
			&_keyMinus,
			&_keyPlus,
			&_keyLeftBrace,
			&_keyRightBrace,
			&_keySlash,
		},
		{
			&_keyShift,
			&_keyAsterisk,
			&_keyDoubleQuote,
			&_keyQuote,
			&_keyColon,
			&_keySemicolon,
			&_keyExclamationMark,
			&_keyQuestionMark,
			&_keyBackspace,
		},
		{
			&_keyCyclicLayout,
			&_keyComma,
			&_keySpace,
			&_keyPeriod,
			&_keyEnter,
		}
	}) {

	}

	// ----------------------------- EnglishKeyboardLayout -----------------------------

	EnglishKeyboardLayout::EnglishKeyboardLayout() : KeyboardLayout({
		{
			&_keyQ,
			&_keyW,
			&_keyE,
			&_keyR,
			&_keyT,
			&_keyY,
			&_keyU,
			&_keyI,
			&_keyO,
			&_keyP,
		},
		{
			&_keyA,
			&_keyS,
			&_keyD,
			&_keyF,
			&_keyG,
			&_keyH,
			&_keyJ,
			&_keyK,
			&_keyL
		},
		{
			&_keyShift,
			&_keyZ,
			&_keyX,
			&_keyC,
			&_keyV,
			&_keyB,
			&_keyN,
			&_keyM,
			&_keyBackspace,
		},
		{
			&_keyCharactersLayout,
			&_keyCyclicLayout,
			&_keyComma,
			&_keySpace,
			&_keyPeriod,
			&_keyEnter,
		}
	}) {

	}

	// ----------------------------- RussianKeyboardLayout -----------------------------

	RussianKeyboardLayout::RussianKeyboardLayout() : KeyboardLayout({
		{
			&_keyQ,
			&_keyW,
			&_keyE,
			&_keyR,
			&_keyT,
			&_keyY,
			&_keyU,
			&_keyI,
			&_keyO,
			&_keyP,
			&_keyBracket
		},
		{
			&_keyA,
			&_keyS,
			&_keyD,
			&_keyF,
			&_keyG,
			&_keyH,
			&_keyJ,
			&_keyK,
			&_keyL,
			&_keySemicolon,
			&_keyQuote
		},
		{
			&_keyShift,
			&_keyZ,
			&_keyX,
			&_keyC,
			&_keyV,
			&_keyB,
			&_keyN,
			&_keyM,
			&_keyLessThan,
			&_keyMoreThan,
			&_keyBackspace,
		},
		{
			&_keyCharactersLayout,
			&_keyCyclicLayout,
			&_keyComma,
			&_keySpace,
			&_keyPeriod,
			&_keyEnter,
		}
	}) {

	}
}