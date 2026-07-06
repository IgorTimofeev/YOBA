#pragma once

#include <cstdint>
#include <span>

#include <YOBA/core/color.h>
#include <YOBA/core/rectangle.h>
#include <YOBA/core/vector2.h>
#include <YOBA/core/size.h>

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
			RenderingTarget(
				const Size& size,
				Rotation rotation,
				PixelOrder pixelOrder,
				ColorModel colorModel
			);

			virtual ~RenderingTarget();

			virtual void setup();
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
			const Size _defaultSize;
			Size _size;
			Rotation _rotation = Rotation::none;
			PixelOrder _pixelOrder;
			ColorModel _colorModel;
			Renderer* _renderer = nullptr;

			void updateSizeFromRotation();
	};
}