#pragma once

#include <cstdint>

#include <YOBA/Core/Size.hpp>
#include <YOBA/Core/Colors.hpp>
#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML
	#include <SFML/Graphics.hpp>
#endif

namespace YOBA {
	class Image {
		public:
			constexpr Image() {
			
			}

			virtual ~Image() = default;

			virtual const Size& getSize() const = 0;
	};

	// I love C++
	namespace EmbeddedImageOptions {
		enum : uint8_t {
			none =      0b00000000,
			alpha1Bit = 0b00000001,
			alpha8Bit = 0b00000010,
		};
	}

	class EmbeddedImage : public Image {
		public:
			constexpr EmbeddedImage(
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

			constexpr const Size& getSize() const override {
				return _size;
			}

			constexpr ColorModel getColorModel() const {
				return _colorModel;
			}

			constexpr uint8_t getOptions() const {
				return _options;
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

	#ifdef YOBA_SYSTEM_SFML
		class SFMLImage : public Image {
			public:
				constexpr SFMLImage() {

				}

				constexpr SFMLImage(sf::Sprite* sprite) : SFMLImage() {
					setSprite(sprite);
				}
				
				const Size& getSize() const override {
					return _size;
				}
				
				void setSprite(sf::Sprite* sprite) {
					_sprite = sprite;

					setSizeMatchingTexture();
				};

				sf::Sprite* getSprite() const {
					return _sprite;
				}
				
				void setSizeMatchingTexture() {
					_size = Size(_sprite->getTexture().getSize().x, _sprite->getTexture().getSize().y);
				}

			private:
				Size _size {};
				
				sf::Sprite* _sprite = nullptr;
		};

	#endif
}