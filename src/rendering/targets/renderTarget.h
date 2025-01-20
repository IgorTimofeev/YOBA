#pragma once

#include <cstdint>
#include "main/color.h"
#include "main/vector.h"
#include "main/size.h"

namespace yoba {
	enum class RenderTargetOrientation : uint8_t {
		clockwise0,
		clockwise90,
		clockwise180,
		clockwise270
	};

	enum class RenderTargetPixelOrder : uint8_t {
		XY,
		YX,

		XYReversed,
		YXReversed,

		XReversedY,
		XReversedX,

		XReversedYReversed,
		XReversedXReversed
	};

	enum class RenderTargetPixelWriting : uint8_t {
		direct,
		buffered
	};

	class RenderTarget {
		public:
			RenderTarget(
				const Size& size,
				RenderTargetOrientation orientation,
				ColorModel colorModel,
				RenderTargetPixelWriting pixelWriting,
				RenderTargetPixelOrder pixelOrder
			);

			virtual void setup();

			RenderTargetPixelWriting getPixelWriting() const;
			RenderTargetPixelOrder getPixelOrder() const;
			ColorModel getColorModel() const;
			const Size& getSize() const;

			RenderTargetOrientation getOrientation() const;
			void setOrientation(RenderTargetOrientation value);

			Point orientPoint(const Point& point);

			bool operator==(const RenderTarget& rhs) const;
			bool operator!=(const RenderTarget& rhs) const;

		protected:
			RenderTargetPixelWriting _pixelWriting;
			RenderTargetPixelOrder _pixelOrder;
			ColorModel _colorModel;
			const Size _defaultSize;
			Size _size;
			RenderTargetOrientation _orientation = RenderTargetOrientation::clockwise270;

			virtual void updateFromOrientation();
			virtual void onOrientationChanged();
	};
}