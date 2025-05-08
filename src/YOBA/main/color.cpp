#include "color.h"
#include "./math.h"

namespace YOBA {
	ColorModel Color::getModel() const {
		return _type;
	}

	// -------------------------------- Color --------------------------------

	Color::Color(ColorModel model) : _type(model) {

	}

	uint8_t Color::getBytesPerModel(ColorModel colorModel) {
		switch (colorModel) {
			case ColorModel::RGB565:
				return 2;

			case ColorModel::RGB666:
			case ColorModel::RGB888:
				return 3;

			default:
				return 1;
		}
	}

	size_t Color::getBytesPerModel(size_t pixelCount, ColorModel colorModel) {
		switch (colorModel) {
			case ColorModel::monochrome:
				return pixelCount >= 8 ? pixelCount / 8 : 1;

			case ColorModel::RGB565:
				return pixelCount * 2;

			case ColorModel::RGB666:
			case ColorModel::RGB888:
				return pixelCount * 3;

			default:
				return pixelCount;
		}
	}

	// -------------------------------- Rgb888Color --------------------------------

	RGB888Color::RGB888Color(uint8_t r, uint8_t g, uint8_t b) :
		Color(ColorModel::RGB888),
		_r(r),
		_g(g),
		_b(b)
	{

	}

	RGB888Color::RGB888Color() : RGB888Color(0, 0, 0) {

	}

	RGB888Color::RGB888Color(uint32_t rgb888) : RGB888Color(
		rgb888 >> 16 & 0xFF,
		rgb888 >> 8 & 0xFF,
		rgb888 & 0xFF
	) {

	}

	uint32_t RGB888Color::toUint32() const {
		return _r << 16 | _g << 8 | _b;
	}

	RGB565Color RGB888Color::toRGB565() const {
		return RGB565Color(_r >> 3 << 3 | _g >> 5 | _g >> 2 << 13 | _b >> 3 << 8);
//		return Rgb565Color(((_r & 0b11111000) << 8) | ((_g & 0b11111100) << 3) | (_b >> 3));
	}

	RGB666Color RGB888Color::toRGB666() const {
		return RGB666Color(_r << 16 | _g << 8 | _b);
	}

	MonochromeColor RGB888Color::toMonochrome() const {
		return MonochromeColor(_r > 0 || _g > 0 || _b > 0);
	}

	void RGB888Color::interpolateTo(const RGB888Color &second, float position) {
		_r = interpolate(_r, second._r, position);
		_g = interpolate(_g, second._g, position);
		_b = interpolate(_b, second._b, position);
	}

	uint8_t RGB888Color::getR() const {
		return _r;
	}

	void RGB888Color::setR(uint8_t r) {
		_r = r;
	}

	uint8_t RGB888Color::getG() const {
		return _g;
	}

	void RGB888Color::setG(uint8_t g) {
		_g = g;
	}

	uint8_t RGB888Color::getB() const {
		return _b;
	}

	void RGB888Color::setB(uint8_t b) {
		_b = b;
	}

	// -------------------------------- HsbColor --------------------------------

	HSBColor::HSBColor(float h, float s, float b) :  Color(ColorModel::HSB), _h(h), _s(s), _b(b) {

	}

	float HSBColor::getH() const {
		return _h;
	}

	void HSBColor::setH(float h) {
		_h = h;
	}

	float HSBColor::getS() const {
		return _s;
	}

	void HSBColor::setS(float s) {
		_s = s;
	}

	float HSBColor::getB() const {
		return _b;
	}

	void HSBColor::setB(float b) {
		_b = b;
	}

	RGB888Color HSBColor::toRgb888() const {
		RGB888Color result;

		const auto hueSector = _h * 6.0f;
		const auto hueSectorIntegerPart = static_cast<uint8_t>(hueSector);
		const auto hueSectorFractionalPart = hueSector - static_cast<float>(hueSectorIntegerPart);

		const auto p = static_cast<uint8_t>(255.0f * _b * (1 - _s));
		const auto q = static_cast<uint8_t>(255.0f * _b * (1 - hueSectorFractionalPart * _s));
		const auto t = static_cast<uint8_t>(255.0f * _b * (1 - (1 - hueSectorFractionalPart) * _s));
		const auto v = static_cast<uint8_t>(255.0f * _b);

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

	MonochromeColor::MonochromeColor(bool value) : ValueColor(ColorModel::monochrome, value) {

	}

	RGB888Color MonochromeColor::toRgb888() const {
		return
			_value > 0
			? RGB888Color(0xFf, 0xFF, 0xFF)
			: RGB888Color(0x00, 0x00, 0x00);
	}

	// -------------------------------- Rgb565Color --------------------------------

	RGB565Color::RGB565Color(uint16_t value) : ValueColor(ColorModel::RGB565, value) {

	}

	RGB888Color RGB565Color::toRgb888() const {
		return {
			static_cast<uint8_t>(((_value >> 11 & 0x1F) * 255 + 15) / 31),
			static_cast<uint8_t>(((_value >> 5 & 0x3F) * 255 + 31) / 63),
			static_cast<uint8_t>(((_value & 0x1F) * 255 + 15) / 31)
		};
	}

	// -------------------------------- Rgb666Color --------------------------------

	RGB666Color::RGB666Color(uint32_t value) : ValueColor(ColorModel::RGB666, value) {

	}

	RGB888Color RGB666Color::toRgb888() const {
		return {
			static_cast<uint8_t>((_value >> 12 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value >> 6 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value & 0b111111) * 255 / 0b111111)
		};
	}

	// -------------------------------- PaletteColor --------------------------------

	Bit8PaletteColor::Bit8PaletteColor(uint16_t index) : PaletteColor(index) {

	}

	Bit8PaletteColor::Bit8PaletteColor() : Bit8PaletteColor(0) {

	}
}