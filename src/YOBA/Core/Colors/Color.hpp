#pragma once

#include <cstdlib>

namespace YOBA {
	enum class ColorModel : uint8_t {
		monochrome,
		RGB565,
		RGB666,
		RGB888,
		ARGB,
		HSB,
		indexed8
	};

	#pragma pack(push, 1)

	class Color {
		public:
			constexpr Color(const ColorModel model) : _model(model) {

			}
			
			constexpr ColorModel getModel() const {
				return _model;
			}

			constexpr static uint8_t getBytesPerModel(const ColorModel colorModel) {
				switch (colorModel) {
					case ColorModel::RGB565:
						return 2;

					case ColorModel::RGB666:
					case ColorModel::RGB888:
						return 3;

					case ColorModel::ARGB:
						return 4;

					default:
						return 1;
				}
			}

			constexpr static size_t getBytesPerModel(const ColorModel colorModel, const size_t pixelCount) {
				switch (colorModel) {
					case ColorModel::monochrome:
						return pixelCount >= 8 ? pixelCount / 8 : 1;

					case ColorModel::RGB565:
						return pixelCount * 2;

					case ColorModel::RGB666:
					case ColorModel::RGB888:
						return pixelCount * 3;

					case ColorModel::ARGB:
						return pixelCount * 4;

					default:
						return pixelCount;
				}
			}

			static const Color* select(
				const bool condition,
				const Color* defaultColor,
				const Color* conditionColor,
				const Color* fallbackColor = nullptr
			) {
				if (condition) {
					if (conditionColor)
						return conditionColor;

					if (defaultColor)
						return defaultColor;
				}
				else if (defaultColor) {
					return defaultColor;
				}

				return fallbackColor;
			}

		private:
			ColorModel _model;
	};

	#pragma pack(pop)
}