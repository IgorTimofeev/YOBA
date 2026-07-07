#pragma once

#include <cstdint>
#include <span>

#include <YOBA/Core/Color.hpp>
#include <YOBA/Core/Rectangle.hpp>
#include <YOBA/Core/Vector2.hpp>
#include <YOBA/Core/Size.hpp>

namespace YOBA {
	enum class Rotation : uint8_t {
		none,
		clockwise90,
		clockwise180,
		clockwise270
	};

	enum class PixelOrder : uint8_t {
		XNormalYNormal,
		YNormalXNormal,

		XNormalYReversed,
		YNormalXReversed,

		XReversedYNormal,
		YReversedXNormal,

		XReversedYReversed,
		YReversedXReversed
	};

	class Renderer;

	class RenderingTarget {
		friend class Renderer;

		public:
			virtual ~RenderingTarget();

			void setup(
				const Size& size,
				Rotation rotation,
				PixelOrder pixelOrder,
				ColorModel colorModel
			);

			virtual void writePixels(const Rectangle& bounds, const std::span<uint8_t> pixelBuffer) = 0;

			PixelOrder getPixelOrder() const;
			ColorModel getColorModel() const;
			const Size& getSize() const;

			Rotation getRotation() const;
			void setRotation(Rotation value);

			Renderer* getRenderer() const;
			Point applyRotation(const Point& point) const;

			bool operator==(const RenderingTarget& rhs) const;
			bool operator!=(const RenderingTarget& rhs) const;

		protected:
			virtual void onRotationChanged();

		private:
			Size _defaultSize {};
			Size _size {};
			Rotation _rotation = Rotation::none;
			PixelOrder _pixelOrder = PixelOrder::XNormalYNormal;
			ColorModel _colorModel = ColorModel::RGB565;
			Renderer* _renderer = nullptr;

			void updateSizeFromRotation();
	};
}