#include "color.h"
#include "Arduino.h"
#include "number.h"

namespace yoba {
	ColorType Color::getType() const {
		return _type;
	}

	// -------------------------------- Color --------------------------------

	Color::Color(ColorType type) : _type(type) {

	}

	// -------------------------------- Rgb888Color --------------------------------

	Rgb888Color::Rgb888Color(uint8_t r, uint8_t g, uint8_t b) :
		Color(ColorType::Rgb888),
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

	uint32_t Rgb888Color::toUint32() const {
		return _r << 16 | _g << 8 | _b;
	}

	Rgb565Color Rgb888Color::toRgb565() const {
		return Rgb565Color(((_r >> 3) << 3) | (_g >> 5) | ((_g >> 2) << 13) | ((_b >> 3) << 8));
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

	// -------------------------------- HsbColor --------------------------------

	HsbColor::HsbColor(float h, float s, float b) :  Color(ColorType::Hsb), _h(h), _s(s), _b(b) {

	}

	float HsbColor::getH() const {
		return _h;
	}

	void HsbColor::setH(float h) {
		_h = h;
	}

	float HsbColor::getS() const {
		return _s;
	}

	void HsbColor::setS(float s) {
		_s = s;
	}

	float HsbColor::getB() const {
		return _b;
	}

	void HsbColor::setB(float b) {
		_b = b;
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

	// -------------------------------- Rgb565Color --------------------------------

	Rgb565Color::Rgb565Color(uint16_t value) : Color(ColorType::Rgb565), _value(value) {

	}

	Rgb565Color::Rgb565Color() : Rgb565Color(0) {

	}

	uint16_t Rgb565Color::getValue() const {
		return _value;
	}

	void Rgb565Color::setValue(uint16_t value) {
		_value = value;
	}

	Rgb888Color Rgb565Color::toRgb888() const {
		return {
			(uint8_t) ((((_value >> 11) & 0x1F) * 255 + 15) / 31),
			(uint8_t) ((((_value >> 5) & 0x3F) * 255 + 31) / 63),
			(uint8_t) ((((_value) & 0x1F) * 255 + 15) / 31)
		};
	}

	// -------------------------------- Bits8PaletteColor --------------------------------

	Bits8PaletteColor::Bits8PaletteColor(uint8_t index) : PaletteColor(index) {

	}
}