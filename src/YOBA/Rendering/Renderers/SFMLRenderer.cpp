#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SFML

#include <YOBA/Rendering/Renderers/SFMLRenderer.hpp>

namespace YOBA {
	void SFMLRenderer::flush() {
		// TODO: rework this shit
		uint8_t pizda = 0;
		_target->flush(Rectangle(_target->getSize()), std::span(&pizda, 1));
	}

	void SFMLRenderer::clearNative(const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		renderTexture.clear(static_cast<const ARGBColor*>(color)->toSFMLColor());
	}

	void SFMLRenderer::fillRectangleNative(const Rectangle& bounds, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(bounds.getX(), bounds.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(bounds.getX() + bounds.getWidth(), bounds.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(bounds.getX(), bounds.getY() + bounds.getHeight()), sfmlColor),
			sf::Vertex(sf::Vector2f(bounds.getX() + bounds.getWidth(), bounds.getY() + bounds.getHeight()), sfmlColor)
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}

	void SFMLRenderer::putImageNative(const Rectangle& bounds, const Image* image) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();

		switch (image->getType()) {
			case ImageType::embedded: {
				const auto embeddedImage = reinterpret_cast<const EmbeddedImage*>(image);

				if (embeddedImage->getColorModel() != ColorModel::ARGB)
					return;

				auto x = bounds.getX();
				auto y = bounds.getY();

				uint16_t imageY;
				uint16_t imageX;

				bool containsY;

				const auto& clip = getClip();
				const auto clipX1 = clip.getX();
				const auto clipY1 = clip.getY();
				const auto clipX2 = clip.getX2();
				const auto clipY2 = clip.getY2();

				auto bitmapPtr = embeddedImage->getBitmap();

				ARGBColor argbColor;

				// With alpha
				if (embeddedImage->getOptions() & EmbeddedImageOptions::alpha8Bit) {
					uint8_t bitmapBitIndex = 0;
					bool containsYX;
					uint8_t alpha;
					uint32_t value24Bit;
					uint8_t* value24BitPtr;

					for (imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
						containsY = y >= clipY1 && y <= clipY2;

						for (imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
							containsYX = containsY && x >= clipX1 && x <= clipX2;

							// 0x00 => alpha < 0xFF
							if (*bitmapPtr & (1 << bitmapBitIndex)) {
								bitmapBitIndex++;

								if (bitmapBitIndex > 7) {
									bitmapBitIndex = 0;
									bitmapPtr++;
								}

								// Decoding alpha value
								alpha = *reinterpret_cast<const uint16_t*>(bitmapPtr) >> bitmapBitIndex;
								bitmapPtr++;

								if (containsYX) {
									value24Bit = *reinterpret_cast<const uint32_t*>(bitmapPtr) >> bitmapBitIndex;
									value24BitPtr = reinterpret_cast<uint8_t*>(&value24Bit);

									argbColor = {
										alpha,
										value24BitPtr[0],
										value24BitPtr[1],
										value24BitPtr[2]
									};

									setPixelNative(Point(x, y), &argbColor);
								}

								bitmapPtr += 3;
							}
							// alpha == 0xFF
							// Pixel can be safely skipped
							else {
								bitmapBitIndex++;

								if (bitmapBitIndex > 7) {
									bitmapBitIndex = 0;
									bitmapPtr++;
								}
							}

							x++;
						}

						x = bounds.getX();
						y++;
					}
				}
				// Without
				else {
					for (imageY = 0; imageY < image->getSize().getHeight(); imageY++) {
						containsY = y >= clipY1 && y <= clipY2;

						for (imageX = 0; imageX < image->getSize().getWidth(); imageX++) {
							if (containsY && x >= clipX1 && x <= clipX2) {
								argbColor = {
									0xFF,
									bitmapPtr[0],
									bitmapPtr[1],
									bitmapPtr[2]
								};

								setPixelNative(Point(x, y), &argbColor);
							}

							x++;

							bitmapPtr += 3;
						}

						x = bounds.getX();
						y++;
					}
				}

				break;
			}
			case ImageType::SFML: {
				const auto sfmlImage = reinterpret_cast<const SFMLImage*>(image);

				if (!sfmlImage->getSprite())
					return;

				sfmlImage->getSprite()->setPosition(sf::Vector2f(bounds.getX(), bounds.getY()));

				sfmlImage->getSprite()->setScale(sf::Vector2f(
					static_cast<float>(bounds.getWidth()) / sfmlImage->getSprite()->getTexture().getSize().x,
					static_cast<float>(bounds.getHeight()) / sfmlImage->getSprite()->getTexture().getSize().y
				));

				renderTexture.draw(*sfmlImage->getSprite());

				break;
			}
		}
	}

	void SFMLRenderer::setPixelNative(const Point& position, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(position.getX(), position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX(), position.getY() + 1), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY() + 1), sfmlColor)
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}

	void SFMLRenderer::strokeHorizontalLineNative(const Point& position, const uint16_t length, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(position.getX(), position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + length, position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX(), position.getY() + 1), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + length, position.getY() + 1), sfmlColor)
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}

	void SFMLRenderer::strokeVerticalLineNative(const Point& position, const uint16_t length, const Color* color) {
		auto& renderTexture = reinterpret_cast<SFMLRenderingTarget*>(_target)->getRenderTexture();
		const auto sfmlColor = static_cast<const ARGBColor*>(color)->toSFMLColor();

		const std::array vertices {
			sf::Vertex(sf::Vector2f(position.getX(), position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX(), position.getY() + length), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY()), sfmlColor),
			sf::Vertex(sf::Vector2f(position.getX() + 1, position.getY() + length), sfmlColor),
		};

		renderTexture.draw(vertices.data(), vertices.size(), sf::PrimitiveType::TriangleStrip);
	}
}

#endif