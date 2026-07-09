#pragma once

#include <cstdint>

#include <YOBA/Core/Size.hpp>

namespace YOBA {
	// I love C++
	namespace ImageOptions {
		enum : uint8_t {
			none =        0b00000000,
			alpha1Bit =   0b00000001,
		};
	}

	class Image {
		public:
			constexpr Image(
				const ColorModel colorModel,
				const uint8_t options,
				const Size& size,
				const uint8_t* bitmap
			) :
				_colorModel(colorModel),
				_options(options),
				_size(size),
				_bitmap(bitmap)
			{
			
			}

			constexpr ColorModel getColorModel() const {
				return _colorModel;
			}

			constexpr uint8_t getOptions() const {
				return _options;
			}

			constexpr const Size& getSize() const {
				return _size;
			}

			constexpr const uint8_t* getBitmap() const {
				return _bitmap;
			}

		private:
			const ColorModel _colorModel;
			const uint8_t _options;
			const Size _size;
			const uint8_t* _bitmap;
	};
}