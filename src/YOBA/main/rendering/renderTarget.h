#pragma once

#include <YOBA/main/color.h>
#include <YOBA/main/bounds.h>
#include <YOBA/main/vector2.h>
#include <YOBA/main/size.h>
#include <YOBA/main/callback.h>

#include <cstdint>

namespace YOBA {
	enum class ViewportRotation : uint8_t {
		clockwise0,
		clockwise90,
		clockwise180,
		clockwise270
	};

	enum class PixelOrder : uint8_t {
		XY,
		YX,

		XYReversed,
		YXReversed,

		XReversedY,
		XReversedX,

		XReversedYReversed,
		XReversedXReversed
	};

	class Renderer;

	class RenderTarget {
		friend class Renderer;

		public:
			RenderTarget(
				const Size& size,
				ViewportRotation rotation,
				PixelOrder pixelOrder,
				ColorModel colorModel
			);

			virtual ~RenderTarget();

			virtual void setup();
			virtual void writePixels(const Bounds& bounds, uint8_t* source, size_t length) = 0;

			PixelOrder getPixelOrder() const;
			ColorModel getColorModel() const;
			const Size& getSize() const;

			ViewportRotation getRotation() const;
			void setRotation(ViewportRotation value);

			Renderer* getRenderer() const;
			Point orientPoint(const Point& point) const;

			bool operator==(const RenderTarget& rhs) const;
			bool operator!=(const RenderTarget& rhs) const;

		protected:
			virtual void onRotationChanged();

		private:
			const Size _defaultSize;
			Size _size;
			ViewportRotation _rotation = ViewportRotation::clockwise0;
			PixelOrder _pixelOrder;
			ColorModel _colorModel;
			Renderer* _renderer = nullptr;

			void updateSizeFromRotation();
	};
}