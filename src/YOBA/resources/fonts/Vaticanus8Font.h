﻿#pragma once

#include <YOBA/main/font.h>

namespace YOBA {
	class Vaticanus8Font : public Font {
		public:
			Vaticanus8Font() : Font(
				32,
				126,
				0,
				8,
				_glyphs,
				_bitmap
			) {

			}

		private:
			constexpr static VariableWidthGlyph _glyphs[95] = {
				VariableWidthGlyph(0, 4), //
				VariableWidthGlyph(32, 2), // !
				VariableWidthGlyph(48, 4), // "
				VariableWidthGlyph(80, 6), // #
				VariableWidthGlyph(128, 4), // $
				VariableWidthGlyph(160, 6), // %
				VariableWidthGlyph(208, 5), // &
				VariableWidthGlyph(248, 2), // '
				VariableWidthGlyph(264, 3), // (
				VariableWidthGlyph(288, 3), // )
				VariableWidthGlyph(312, 4), // *
				VariableWidthGlyph(344, 4), // +
				VariableWidthGlyph(376, 3), // ,
				VariableWidthGlyph(400, 4), // -
				VariableWidthGlyph(432, 2), // .
				VariableWidthGlyph(448, 6), // /
				VariableWidthGlyph(496, 4), // 0
				VariableWidthGlyph(528, 4), // 1
				VariableWidthGlyph(560, 4), // 2
				VariableWidthGlyph(592, 4), // 3
				VariableWidthGlyph(624, 4), // 4
				VariableWidthGlyph(656, 4), // 5
				VariableWidthGlyph(688, 4), // 6
				VariableWidthGlyph(720, 4), // 7
				VariableWidthGlyph(752, 4), // 8
				VariableWidthGlyph(784, 4), // 9
				VariableWidthGlyph(816, 2), // :
				VariableWidthGlyph(832, 3), // ;
				VariableWidthGlyph(856, 4), // <
				VariableWidthGlyph(888, 4), // =
				VariableWidthGlyph(920, 4), // >
				VariableWidthGlyph(952, 4), // ?
				VariableWidthGlyph(984, 5), // @
				VariableWidthGlyph(1024, 4), // A
				VariableWidthGlyph(1056, 4), // B
				VariableWidthGlyph(1088, 4), // C
				VariableWidthGlyph(1120, 4), // D
				VariableWidthGlyph(1152, 4), // E
				VariableWidthGlyph(1184, 4), // F
				VariableWidthGlyph(1216, 5), // G
				VariableWidthGlyph(1256, 4), // H
				VariableWidthGlyph(1288, 4), // I
				VariableWidthGlyph(1320, 4), // J
				VariableWidthGlyph(1352, 4), // K
				VariableWidthGlyph(1384, 4), // L
				VariableWidthGlyph(1416, 6), // M
				VariableWidthGlyph(1464, 5), // N
				VariableWidthGlyph(1504, 4), // O
				VariableWidthGlyph(1536, 4), // P
				VariableWidthGlyph(1568, 4), // Q
				VariableWidthGlyph(1600, 4), // R
				VariableWidthGlyph(1632, 4), // S
				VariableWidthGlyph(1664, 4), // T
				VariableWidthGlyph(1696, 4), // U
				VariableWidthGlyph(1728, 4), // V
				VariableWidthGlyph(1760, 6), // W
				VariableWidthGlyph(1808, 4), // X
				VariableWidthGlyph(1840, 4), // Y
				VariableWidthGlyph(1872, 4), // Z
				VariableWidthGlyph(1904, 3), // [
				VariableWidthGlyph(1928, 6), // Backslash
				VariableWidthGlyph(1976, 3), // ]
				VariableWidthGlyph(2000, 4), // ^
				VariableWidthGlyph(2032, 4), // _
				VariableWidthGlyph(2064, 3), // `
				VariableWidthGlyph(2088, 4), // a
				VariableWidthGlyph(2120, 4), // b
				VariableWidthGlyph(2152, 4), // c
				VariableWidthGlyph(2184, 4), // d
				VariableWidthGlyph(2216, 4), // e
				VariableWidthGlyph(2248, 4), // f
				VariableWidthGlyph(2280, 4), // g
				VariableWidthGlyph(2312, 4), // h
				VariableWidthGlyph(2344, 2), // i
				VariableWidthGlyph(2360, 3), // j
				VariableWidthGlyph(2384, 4), // k
				VariableWidthGlyph(2416, 2), // l
				VariableWidthGlyph(2432, 6), // m
				VariableWidthGlyph(2480, 4), // n
				VariableWidthGlyph(2512, 4), // o
				VariableWidthGlyph(2544, 4), // p
				VariableWidthGlyph(2576, 4), // q
				VariableWidthGlyph(2608, 4), // r
				VariableWidthGlyph(2640, 4), // s
				VariableWidthGlyph(2672, 4), // t
				VariableWidthGlyph(2704, 4), // u
				VariableWidthGlyph(2736, 4), // v
				VariableWidthGlyph(2768, 6), // w
				VariableWidthGlyph(2816, 4), // x
				VariableWidthGlyph(2848, 4), // y
				VariableWidthGlyph(2880, 4), // z
				VariableWidthGlyph(2912, 4), // {
				VariableWidthGlyph(2944, 2), // |
				VariableWidthGlyph(2960, 4), // }
				VariableWidthGlyph(2992, 5) // ~
			};

			constexpr static uint8_t _bitmap[379] = {
				0x00, 0x00, 0x00, 0x00, 0x54, 0x04, 0x55, 0x00, 0x00, 0x00, 0x80, 0xF2, 0x29, 0x9F, 0x02, 0x00,
				0x62, 0x71, 0x34, 0x02, 0x40, 0x84, 0x10, 0x42, 0x04, 0x00, 0xE0, 0x14, 0x51, 0x16, 0x00, 0x05,
				0x00, 0x50, 0x12, 0x01, 0x88, 0xA4, 0x00, 0x20, 0x27, 0x05, 0x00, 0x00, 0x72, 0x02, 0x00, 0x00,
				0x00, 0x05, 0x00, 0x70, 0x00, 0x00, 0x00, 0x04, 0x00, 0x84, 0x10, 0x42, 0x00, 0x00, 0x70, 0x55,
				0x75, 0x00, 0x20, 0x23, 0x72, 0x00, 0x70, 0x74, 0x71, 0x00, 0x70, 0x64, 0x74, 0x00, 0x50, 0x75,
				0x44, 0x00, 0x70, 0x71, 0x74, 0x00, 0x10, 0x71, 0x75, 0x00, 0x70, 0x44, 0x44, 0x00, 0x70, 0x75,
				0x75, 0x00, 0x70, 0x75, 0x44, 0x00, 0x10, 0x01, 0x80, 0x00, 0x05, 0x40, 0x12, 0x42, 0x00, 0x00,
				0x07, 0x07, 0x00, 0x10, 0x42, 0x12, 0x00, 0x70, 0x24, 0x20, 0x00, 0xE0, 0x20, 0xD5, 0x0C, 0x00,
				0x20, 0x75, 0x55, 0x00, 0x30, 0x35, 0x35, 0x00, 0x70, 0x11, 0x71, 0x00, 0x30, 0x55, 0x35, 0x00,
				0x70, 0x31, 0x71, 0x00, 0x70, 0x31, 0x11, 0x00, 0xC0, 0x84, 0x96, 0x1C, 0x00, 0x50, 0x75, 0x55,
				0x00, 0x70, 0x22, 0x72, 0x00, 0x40, 0x44, 0x75, 0x00, 0x50, 0x35, 0x55, 0x00, 0x10, 0x11, 0x71,
				0x00, 0x40, 0xB4, 0x55, 0x51, 0x04, 0x00, 0x20, 0xAD, 0x96, 0x12, 0x00, 0x20, 0x55, 0x25, 0x00,
				0x30, 0x35, 0x11, 0x00, 0x20, 0x55, 0x63, 0x00, 0x70, 0x35, 0x55, 0x00, 0x60, 0x71, 0x34, 0x00,
				0x70, 0x22, 0x22, 0x00, 0x50, 0x55, 0x75, 0x00, 0x50, 0x55, 0x22, 0x00, 0x40, 0x55, 0x55, 0x95,
				0x02, 0x00, 0x50, 0x25, 0x55, 0x00, 0x50, 0x25, 0x22, 0x00, 0x70, 0x24, 0x71, 0x00, 0x58, 0x92,
				0x01, 0x40, 0x20, 0x10, 0x08, 0x04, 0x00, 0x98, 0xA4, 0x01, 0x20, 0x55, 0x00, 0x00, 0x00, 0x00,
				0x00, 0x07, 0x88, 0x00, 0x00, 0x00, 0x56, 0x65, 0x00, 0x10, 0x53, 0x35, 0x00, 0x00, 0x16, 0x61,
				0x00, 0x40, 0x56, 0x65, 0x00, 0x00, 0x52, 0x63, 0x00, 0x60, 0x31, 0x11, 0x00, 0x00, 0x56, 0x65,
				0x03, 0x10, 0x53, 0x55, 0x00, 0x44, 0x05, 0x10, 0x24, 0x05, 0x10, 0x35, 0x55, 0x00, 0x54, 0x05,
				0x00, 0xB0, 0x54, 0x55, 0x05, 0x00, 0x00, 0x53, 0x55, 0x00, 0x00, 0x52, 0x25, 0x00, 0x00, 0x53,
				0x35, 0x01, 0x00, 0x56, 0x65, 0x04, 0x00, 0x53, 0x11, 0x00, 0x00, 0x36, 0x34, 0x00, 0x20, 0x27,
				0x22, 0x00, 0x00, 0x55, 0x65, 0x00, 0x00, 0x55, 0x22, 0x00, 0x00, 0x50, 0x55, 0x95, 0x02, 0x00,
				0x00, 0x25, 0x55, 0x00, 0x00, 0x55, 0x65, 0x03, 0x00, 0x47, 0x72, 0x00, 0x60, 0x32, 0x62, 0x00,
				0x55, 0x15, 0x30, 0x62, 0x32, 0x00, 0x40, 0x15, 0x00, 0x00, 0x00,
			};
	};
}