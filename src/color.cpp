#include "color.h"
#include "Arduino.h"
#include "number.h"

namespace yoba {
	Color::Color(ColorType type) : _type(type) {

	}

	ColorType Color::getType() const {
		return _type;
	}

	Rgb888Color::Rgb888Color(uint8_t r, uint8_t g, uint8_t b) :
		Color(ColorType::RGB888),
		_r(r),
		_g(g),
		_b(b)
	{

	}

	Rgb888Color::Rgb888Color() : Rgb888Color(0, 0, 0) {

	}

	Rgb888Color::Rgb888Color(const Rgb888Color &source) : Rgb888Color(
		source._r,
		source._g,
		source._b
	)  {

	}


	Rgb888Color::Rgb888Color(uint32_t rgb888) : Rgb888Color(
		rgb888 >> 16 & 0xFF,
		rgb888 >> 8 & 0xFF,
		rgb888 & 0xFF
	) {

	}

	Rgb888Color::Rgb888Color(uint16_t rgb565) : Rgb888Color(
		(uint8_t) ((((rgb565 >> 11) & 0x1F) * 255 + 15) / 31),
		(uint8_t) ((((rgb565 >> 5) & 0x3F) * 255 + 31) / 63),
		(uint8_t) ((((rgb565) & 0x1F) * 255 + 15) / 31)
	) {

	}

	uint32_t Rgb888Color::toUint32() const {
		return _r << 16 | _g << 8 | _b;
	}

	uint16_t Rgb888Color::toUint16() const {
		return ((_r >> 3) << 3) | (_g >> 5) | ((_g >> 2) << 13) | ((_b >> 3) << 8);
	}

	void Rgb888Color::interpolateTo(Rgb888Color &second, float position) {
		_r = interpolate(_r, second._r, position);
		_g = interpolate(_g, second._g, position);
		_b = interpolate(_b, second._b, position);
	}

	uint8_t Rgb888Color::getR() const {
		return _r;
	}

	void Rgb888Color::setR(uint8_t r) {
		_r = r;
	}

	uint8_t Rgb888Color::getG() const {
		return _g;
	}

	void Rgb888Color::setG(uint8_t g) {
		_g = g;
	}

	uint8_t Rgb888Color::getB() const {
		return _b;
	}

	void Rgb888Color::setB(uint8_t b) {
		_b = b;
	}

	// ------------------------------------------------------------------------------------

	HsbColor::HsbColor(float h, float s, float b) :  Color(ColorType::HSB), _h(h), _s(s), _b(b) {

	}

	Rgb888Color HsbColor::toRgb888() const {
		Rgb888Color result;

		auto hueSector = _h * 6.0f;
		auto hueSectorIntegerPart = (uint8_t) hueSector;
		auto hueSectorFractionalPart = hueSector - (float) hueSectorIntegerPart;

		auto
			p = (uint8_t) (255.0f * _b * (1 - _s)),
			q = (uint8_t) (255.0f * _b * (1 - hueSectorFractionalPart * _s)),
			t = (uint8_t) (255.0f * _b * (1 - (1 - hueSectorFractionalPart) * _s)),
			v = (uint8_t) (255.0f * _b);

		switch (hueSectorIntegerPart) {
			case 1:
				result.setR(q);
				result.setG(v);
				result.setB(p);
				break;

			case 2:
				result.setR(p);
				result.setG(v);
				result.setB(t);
				break;

			case 3:
				result.setR(p);
				result.setG(q);
				result.setB(v);
				break;

			case 4:
				result.setR(t);
				result.setG(p);
				result.setB(v);
				break;

			case 5:
				result.setR(v);
				result.setG(p);
				result.setB(q);
				break;

			default:
				result.setR(v);
				result.setG(t);
				result.setB(p);
				break;
		}

		return result;
	}

	// ------------------------------------------------------------------------------------

	PaletteColor::PaletteColor(uint8_t index) : Color(ColorType::Palette), _index(index) {

	}

	uint8_t PaletteColor::getIndex() const {
		return _index;
	}

	void PaletteColor::setIndex(uint8_t index) {
		_index = index;
	}
}