#include "color.h"
#include "math.h"

namespace yoba {
	ColorModel Color::getModel() const {
		return _type;
	}

	// -------------------------------- Color --------------------------------

	Color::Color(ColorModel model) : _type(model) {

	}

	uint8_t Color::getBytesPerType(ColorModel colorModel) {
		switch (colorModel) {
			case ColorModel::rgb565:
				return 2;

			case ColorModel::rgb666:
			case ColorModel::rgb888:
				return 3;

			default:
				return 1;
		}
	}

	size_t Color::getBytesPerType(size_t pixelsCount, ColorModel colorModel) {
		switch (colorModel) {
			case ColorModel::monochrome:
				return pixelsCount >= 8 ? pixelsCount / 8 : 1;

			case ColorModel::rgb565:
				return pixelsCount * 2;

			case ColorModel::rgb666:
			case ColorModel::rgb888:
				return pixelsCount * 3;

			default:
				return pixelsCount;
		}
	}

	// -------------------------------- Rgb888Color --------------------------------

	Rgb888Color::Rgb888Color(uint8_t r, uint8_t g, uint8_t b) :
		Color(ColorModel::rgb888),
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

	Rgb666Color Rgb888Color::toRgb666() const {
		return Rgb666Color((_r << 16) | (_g << 8) | _b);
	}

	MonochromeColor Rgb888Color::toMonochrome() const {
		return MonochromeColor(_r > 0 || _g > 0 || _b > 0);
	}

	void Rgb888Color::interpolateTo(const Rgb888Color &second, float position) {
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

	HsbColor::HsbColor(float h, float s, float b) :  Color(ColorModel::hsb), _h(h), _s(s), _b(b) {

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

	// -------------------------------- MonochromeColor --------------------------------

	MonochromeColor::MonochromeColor(bool value) : ValueColor<bool>(ColorModel::monochrome, value) {

	}

	Rgb888Color MonochromeColor::toRgb888() const {
		return
			_value > 0
			? Rgb888Color(0xFf, 0xFF, 0xFF)
			: Rgb888Color(0x00, 0x00, 0x00);
	}

	// -------------------------------- Rgb565Color --------------------------------

	Rgb565Color::Rgb565Color(uint16_t value) : ValueColor<uint16_t>(ColorModel::rgb565, value) {

	}

	Rgb888Color Rgb565Color::toRgb888() const {
		return {
			(uint8_t) ((((_value >> 11) & 0x1F) * 255 + 15) / 31),
			(uint8_t) ((((_value >> 5) & 0x3F) * 255 + 31) / 63),
			(uint8_t) (((_value & 0x1F) * 255 + 15) / 31)
		};
	}

	// -------------------------------- Rgb666Color --------------------------------

	Rgb666Color::Rgb666Color(uint32_t value) : ValueColor<uint32_t>(ColorModel::rgb666, value) {

	}

	Rgb888Color Rgb666Color::toRgb888() const {
		return {
			(uint8_t) (((_value >> 12) & 0b111111) * 255 / 0b111111),
			(uint8_t) (((_value >> 6) & 0b111111) * 255 / 0b111111),
			(uint8_t) ((_value & 0b111111) * 255 / 0b111111)
		};
	}

	// -------------------------------- PaletteColor --------------------------------

	PaletteColor::PaletteColor(uint16_t index) : Color(ColorModel::palette), _index(index) {

	}

	PaletteColor::PaletteColor() : PaletteColor(0) {

	}

	uint16_t PaletteColor::getIndex() const {
		return _index;
	}

	void PaletteColor::setIndex(uint16_t index) {
		_index = index;
	}
}