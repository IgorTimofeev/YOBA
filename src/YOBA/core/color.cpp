#include "color.h"

#include "math.h"

namespace YOBA {
	ColorModel Color::getModel() const {
		return _model;
	}

	// -------------------------------- Color --------------------------------

	uint8_t Color::getBytesPerModel(const ColorModel colorModel) {
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

	size_t Color::getBytesPerModel(const size_t pixelCount, const ColorModel colorModel) {
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

	// -------------------------------- RGB888Color --------------------------------

	RGB666Color RGB888Color::toRGB666() const {
		return RGB666Color(_r << 16 | _g << 8 | _b);
	}

	MonochromeColor RGB888Color::toMonochrome() const {
		return MonochromeColor(_r > 0 || _g > 0 || _b > 0);
	}

	void RGB888Color::interpolateTo(const RGB888Color &second, const float position) {
		_r = static_cast<uint8_t>(interpolate(static_cast<float>(_r), static_cast<float>(second._r), position));
		_g = static_cast<uint8_t>(interpolate(static_cast<float>(_g), static_cast<float>(second._g), position));
		_b = static_cast<uint8_t>(interpolate(static_cast<float>(_b), static_cast<float>(second._b), position));
	}

	// -------------------------------- MonochromeColor --------------------------------

	RGB888Color MonochromeColor::toRGB888() const {
		return
			_value > 0
			? RGB888Color(0xFf, 0xFF, 0xFF)
			: RGB888Color(0x00, 0x00, 0x00);
	}

	// -------------------------------- Rgb565Color --------------------------------

	RGB888Color RGB565Color::toRGB888() const {
		return {
			static_cast<uint8_t>(((_value >> 11 & 0x1F) * 255 + 15) / 31),
			static_cast<uint8_t>(((_value >> 5 & 0x3F) * 255 + 31) / 63),
			static_cast<uint8_t>(((_value & 0x1F) * 255 + 15) / 31)
		};
	}

	// -------------------------------- Rgb666Color --------------------------------

	RGB888Color RGB666Color::toRGB888() const {
		return {
			static_cast<uint8_t>((_value >> 12 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value >> 6 & 0b111111) * 255 / 0b111111),
			static_cast<uint8_t>((_value & 0b111111) * 255 / 0b111111)
		};
	}

	// -------------------------------- PaletteColor --------------------------------

	// -------------------------------- HSBColor --------------------------------


}