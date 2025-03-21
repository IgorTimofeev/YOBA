﻿#pragma once

#include "main/font.h"

namespace yoba {
	class UNSCII8Font : public Font {
		public:
			UNSCII8Font() : Font(
				32,
				126,
				9,
				_glyphs,
				_bitmap
			) {

			}

		private:
			const Glyph _glyphs[95] = {
				Glyph(0, 8), //
				Glyph(72, 8), // !
				Glyph(144, 8), // "
				Glyph(216, 8), // #
				Glyph(288, 8), // $
				Glyph(360, 8), // %
				Glyph(432, 8), // &
				Glyph(504, 8), // '
				Glyph(576, 8), // (
				Glyph(648, 8), // )
				Glyph(720, 8), // *
				Glyph(792, 8), // +
				Glyph(864, 8), // ,
				Glyph(936, 8), // -
				Glyph(1008, 8), // .
				Glyph(1080, 8), // /
				Glyph(1152, 8), // 0
				Glyph(1224, 8), // 1
				Glyph(1296, 8), // 2
				Glyph(1368, 8), // 3
				Glyph(1440, 8), // 4
				Glyph(1512, 8), // 5
				Glyph(1584, 8), // 6
				Glyph(1656, 8), // 7
				Glyph(1728, 8), // 8
				Glyph(1800, 8), // 9
				Glyph(1872, 8), // :
				Glyph(1944, 8), // ;
				Glyph(2016, 8), // <
				Glyph(2088, 8), // =
				Glyph(2160, 8), // >
				Glyph(2232, 8), // ?
				Glyph(2304, 8), // @
				Glyph(2376, 8), // A
				Glyph(2448, 8), // B
				Glyph(2520, 8), // C
				Glyph(2592, 8), // D
				Glyph(2664, 8), // E
				Glyph(2736, 8), // F
				Glyph(2808, 8), // G
				Glyph(2880, 8), // H
				Glyph(2952, 8), // I
				Glyph(3024, 8), // J
				Glyph(3096, 8), // K
				Glyph(3168, 8), // L
				Glyph(3240, 8), // M
				Glyph(3312, 8), // N
				Glyph(3384, 8), // O
				Glyph(3456, 8), // P
				Glyph(3528, 8), // Q
				Glyph(3600, 8), // R
				Glyph(3672, 8), // S
				Glyph(3744, 8), // T
				Glyph(3816, 8), // U
				Glyph(3888, 8), // V
				Glyph(3960, 8), // W
				Glyph(4032, 8), // X
				Glyph(4104, 8), // Y
				Glyph(4176, 8), // Z
				Glyph(4248, 8), // [
				Glyph(4320, 8), // backslash
				Glyph(4392, 8), // ]
				Glyph(4464, 8), // ^
				Glyph(4536, 8), // _
				Glyph(4608, 8), // `
				Glyph(4680, 8), // a
				Glyph(4752, 8), // b
				Glyph(4824, 8), // c
				Glyph(4896, 8), // d
				Glyph(4968, 8), // e
				Glyph(5040, 8), // f
				Glyph(5112, 8), // g
				Glyph(5184, 8), // h
				Glyph(5256, 8), // i
				Glyph(5328, 8), // j
				Glyph(5400, 8), // k
				Glyph(5472, 8), // l
				Glyph(5544, 8), // m
				Glyph(5616, 8), // n
				Glyph(5688, 8), // o
				Glyph(5760, 8), // p
				Glyph(5832, 8), // q
				Glyph(5904, 8), // r
				Glyph(5976, 8), // s
				Glyph(6048, 8), // t
				Glyph(6120, 8), // u
				Glyph(6192, 8), // v
				Glyph(6264, 8), // w
				Glyph(6336, 8), // x
				Glyph(6408, 8), // y
				Glyph(6480, 8), // z
				Glyph(6552, 8), // {
				Glyph(6624, 8), // |
				Glyph(6696, 8), // }
				Glyph(6768, 8) // ~
			};

			const uint8_t _bitmap[855] = {
				0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x08,
				0x00, 0x00, 0x24, 0x24, 0x24, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x24, 0x24, 0x7E, 0x24, 0x7E,
				0x24, 0x24, 0x00, 0x00, 0x08, 0x3C, 0x0A, 0x1C, 0x28, 0x1E, 0x08, 0x00, 0x00, 0x00, 0x46, 0x26,
				0x10, 0x08, 0x64, 0x62, 0x00, 0x00, 0x0C, 0x12, 0x12, 0x0C, 0x52, 0x22, 0x5C, 0x00, 0x00, 0x10,
				0x08, 0x04, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x10, 0x08, 0x04, 0x04, 0x04, 0x08, 0x10, 0x00,
				0x00, 0x04, 0x08, 0x10, 0x10, 0x10, 0x08, 0x04, 0x00, 0x00, 0x08, 0x2A, 0x1C, 0x08, 0x1C, 0x2A,
				0x08, 0x00, 0x00, 0x00, 0x08, 0x08, 0x3E, 0x08, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x10, 0x10, 0x08, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x40, 0x20, 0x10, 0x08, 0x04, 0x02, 0x01, 0x00, 0x00,
				0x1C, 0x22, 0x32, 0x2A, 0x26, 0x22, 0x1C, 0x00, 0x00, 0x08, 0x0C, 0x0A, 0x08, 0x08, 0x08, 0x3E,
				0x00, 0x00, 0x1C, 0x22, 0x20, 0x10, 0x0C, 0x02, 0x3E, 0x00, 0x00, 0x1C, 0x22, 0x20, 0x18, 0x20,
				0x22, 0x1C, 0x00, 0x00, 0x10, 0x18, 0x14, 0x12, 0x3E, 0x10, 0x10, 0x00, 0x00, 0x3E, 0x02, 0x1E,
				0x20, 0x20, 0x22, 0x1C, 0x00, 0x00, 0x38, 0x04, 0x02, 0x1E, 0x22, 0x22, 0x1C, 0x00, 0x00, 0x3E,
				0x20, 0x10, 0x08, 0x04, 0x04, 0x04, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x1C, 0x22, 0x22, 0x1C, 0x00,
				0x00, 0x1C, 0x22, 0x22, 0x3C, 0x20, 0x10, 0x0E, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08,
				0x00, 0x00, 0x00, 0x00, 0x00, 0x08, 0x00, 0x00, 0x08, 0x08, 0x04, 0x00, 0x10, 0x08, 0x04, 0x02,
				0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00, 0x3E, 0x00, 0x00, 0x00, 0x00, 0x04, 0x08,
				0x10, 0x20, 0x10, 0x08, 0x04, 0x00, 0x00, 0x3C, 0x42, 0x20, 0x10, 0x10, 0x00, 0x10, 0x00, 0x00,
				0x3C, 0x42, 0x52, 0x6A, 0x32, 0x02, 0x3C, 0x00, 0x00, 0x18, 0x24, 0x42, 0x7E, 0x42, 0x42, 0x42,
				0x00, 0x00, 0x3E, 0x42, 0x42, 0x3E, 0x42, 0x42, 0x3E, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x02, 0x02,
				0x42, 0x3C, 0x00, 0x00, 0x1E, 0x22, 0x42, 0x42, 0x42, 0x22, 0x1E, 0x00, 0x00, 0x7E, 0x02, 0x02,
				0x1E, 0x02, 0x02, 0x7E, 0x00, 0x00, 0x7E, 0x02, 0x02, 0x1E, 0x02, 0x02, 0x02, 0x00, 0x00, 0x3C,
				0x42, 0x02, 0x72, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x42, 0x42, 0x42, 0x7E, 0x42, 0x42, 0x42, 0x00,
				0x00, 0x1C, 0x08, 0x08, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00, 0x20, 0x20, 0x20, 0x20, 0x20, 0x22,
				0x1C, 0x00, 0x00, 0x42, 0x22, 0x12, 0x0E, 0x12, 0x22, 0x42, 0x00, 0x00, 0x02, 0x02, 0x02, 0x02,
				0x02, 0x02, 0x7E, 0x00, 0x00, 0x42, 0x66, 0x5A, 0x42, 0x42, 0x42, 0x42, 0x00, 0x00, 0x42, 0x46,
				0x4A, 0x52, 0x62, 0x42, 0x42, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x42, 0x42, 0x3C, 0x00, 0x00,
				0x3E, 0x42, 0x42, 0x3E, 0x02, 0x02, 0x02, 0x00, 0x00, 0x3C, 0x42, 0x42, 0x42, 0x52, 0x22, 0x5C,
				0x00, 0x00, 0x3E, 0x42, 0x42, 0x3E, 0x12, 0x22, 0x42, 0x00, 0x00, 0x3C, 0x42, 0x02, 0x3C, 0x40,
				0x42, 0x3C, 0x00, 0x00, 0x3E, 0x08, 0x08, 0x08, 0x08, 0x08, 0x08, 0x00, 0x00, 0x42, 0x42, 0x42,
				0x42, 0x42, 0x42, 0x3C, 0x00, 0x00, 0x42, 0x42, 0x42, 0x42, 0x42, 0x24, 0x18, 0x00, 0x00, 0x42,
				0x42, 0x42, 0x42, 0x5A, 0x66, 0x42, 0x00, 0x00, 0x42, 0x42, 0x24, 0x18, 0x24, 0x42, 0x42, 0x00,
				0x00, 0x22, 0x22, 0x22, 0x1C, 0x08, 0x08, 0x08, 0x00, 0x00, 0x3E, 0x20, 0x10, 0x08, 0x04, 0x02,
				0x3E, 0x00, 0x00, 0x1C, 0x04, 0x04, 0x04, 0x04, 0x04, 0x1C, 0x00, 0x00, 0x01, 0x02, 0x04, 0x08,
				0x10, 0x20, 0x40, 0x00, 0x00, 0x1C, 0x10, 0x10, 0x10, 0x10, 0x10, 0x1C, 0x00, 0x00, 0x08, 0x14,
				0x22, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x3E, 0x00,
				0x04, 0x08, 0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x20, 0x3C, 0x22, 0x3C,
				0x00, 0x00, 0x02, 0x02, 0x1E, 0x22, 0x22, 0x22, 0x1E, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x02,
				0x22, 0x1C, 0x00, 0x00, 0x20, 0x20, 0x3C, 0x22, 0x22, 0x22, 0x3C, 0x00, 0x00, 0x00, 0x00, 0x1C,
				0x22, 0x3E, 0x02, 0x1C, 0x00, 0x00, 0x30, 0x48, 0x08, 0x3E, 0x08, 0x08, 0x08, 0x00, 0x00, 0x00,
				0x00, 0x3C, 0x22, 0x22, 0x3C, 0x20, 0x1C, 0x00, 0x02, 0x02, 0x1E, 0x22, 0x22, 0x22, 0x22, 0x00,
				0x00, 0x08, 0x00, 0x0C, 0x08, 0x08, 0x08, 0x1C, 0x00, 0x00, 0x20, 0x00, 0x30, 0x20, 0x20, 0x20,
				0x24, 0x18, 0x00, 0x02, 0x02, 0x22, 0x12, 0x0E, 0x12, 0x22, 0x00, 0x00, 0x0C, 0x08, 0x08, 0x08,
				0x08, 0x08, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x16, 0x2A, 0x2A, 0x2A, 0x2A, 0x00, 0x00, 0x00, 0x00,
				0x1E, 0x22, 0x22, 0x22, 0x22, 0x00, 0x00, 0x00, 0x00, 0x1C, 0x22, 0x22, 0x22, 0x1C, 0x00, 0x00,
				0x00, 0x00, 0x1E, 0x22, 0x22, 0x1E, 0x02, 0x02, 0x00, 0x00, 0x00, 0x3C, 0x22, 0x22, 0x3C, 0x20,
				0x20, 0x00, 0x00, 0x00, 0x3A, 0x06, 0x02, 0x02, 0x02, 0x00, 0x00, 0x00, 0x00, 0x3C, 0x02, 0x1C,
				0x20, 0x1E, 0x00, 0x00, 0x04, 0x04, 0x1E, 0x04, 0x04, 0x24, 0x18, 0x00, 0x00, 0x00, 0x00, 0x22,
				0x22, 0x22, 0x22, 0x1C, 0x00, 0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x14, 0x08, 0x00, 0x00, 0x00,
				0x00, 0x22, 0x2A, 0x2A, 0x2A, 0x14, 0x00, 0x00, 0x00, 0x00, 0x22, 0x14, 0x08, 0x14, 0x22, 0x00,
				0x00, 0x00, 0x00, 0x22, 0x22, 0x22, 0x3C, 0x20, 0x1C, 0x00, 0x00, 0x00, 0x3E, 0x10, 0x08, 0x04,
				0x3E, 0x00, 0x00, 0x18, 0x04, 0x04, 0x02, 0x04, 0x04, 0x18, 0x00, 0x00, 0x08, 0x08, 0x08, 0x08,
				0x08, 0x08, 0x08, 0x08, 0x00, 0x0C, 0x10, 0x10, 0x20, 0x10, 0x10, 0x0C, 0x00, 0x00, 0x04, 0x2A,
				0x10, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00,
			};
	};
}