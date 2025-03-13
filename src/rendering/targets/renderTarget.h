#pragma once

#include <cstdint>
#include "main/color.h"
#include "main/vector2.h"
#include "main/size.h"

namespace yoba {
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

	enum class PixelWriting : uint8_t {
		direct,
		buffered
	};

	class RenderTarget {
		public:
			RenderTarget(
				const Size& size,
				PixelWriting pixelWriting,
				PixelOrder pixelOrder,
				ColorModel colorModel,
				ViewportRotation rotation
			);

			virtual void setup();

			PixelWriting getPixelWriting() const;
			PixelOrder getPixelOrder() const;
			ColorModel getColorModel() const;
			const Size& getSize() const;

			ViewportRotation getRotation() const;
			void setRotation(ViewportRotation value);

			Point orientPoint(const Point& point);

			bool operator==(const RenderTarget& rhs) const;
			bool operator!=(const RenderTarget& rhs) const;

		protected:
			const Size _defaultSize;
			Size _size;
			PixelWriting _pixelWriting;
			PixelOrder _pixelOrder;
			ColorModel _colorModel;
			ViewportRotation _rotation = ViewportRotation::clockwise0;

			virtual void updateFromRotation();
			virtual void onRotationChanged();
	};
}