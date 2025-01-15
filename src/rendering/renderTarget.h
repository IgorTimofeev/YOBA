#pragma once

#include <cstdint>
#include "color.h"
#include "vector.h"
#include "size.h"

namespace yoba {
	enum class RenderTargetOrientation : uint8_t {
		Clockwise0,
		Clockwise90,
		Clockwise180,
		Clockwise270
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
		Direct,
		Buffered
	};

	class RenderTarget {
		public:
			RenderTarget(
				RenderTargetPixelWriting pixelWriting,
				RenderTargetPixelOrder pixelOrder,
				ColorModel colorModel,
				const Size& resolution,
				RenderTargetOrientation orientation
			);

			virtual void setup();

			RenderTargetPixelWriting getPixelWriting() const;
			RenderTargetPixelOrder getPixelOrder() const;
			ColorModel getColorModel() const;
			const Size& getResolution() const;

			RenderTargetOrientation getOrientation() const;
			void setOrientation(RenderTargetOrientation orientation);

			Point orientPoint(const Point& point);

		protected:
			RenderTargetPixelWriting _pixelWriting;
			RenderTargetPixelOrder _pixelOrder;
			ColorModel _colorModel;
			const Size _defaultResolution;
			Size _resolution;
			RenderTargetOrientation _orientation = RenderTargetOrientation::Clockwise270;

			virtual void updateFromOrientation();
			virtual void onOrientationChanged();
	};
}