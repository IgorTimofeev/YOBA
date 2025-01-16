#pragma once

#include <cstdint>
#include "color.h"
#include "vector.h"
#include "size.h"

namespace yoba {
	enum class RenderingOrientation : uint8_t {
		Clockwise0,
		Clockwise90,
		Clockwise180,
		Clockwise270
	};

	enum class RenderingPixelOrder : uint8_t {
		XY,
		YX,

		XYReversed,
		YXReversed,

		XReversedY,
		XReversedX,

		XReversedYReversed,
		XReversedXReversed
	};

	enum class RenderingPixelWriting : uint8_t {
		Direct,
		Buffered
	};

	class RenderTarget {
		public:
			RenderTarget(
				const Size& resolution,
				RenderingOrientation orientation,
				ColorModel colorModel,
				RenderingPixelWriting pixelWriting,
				RenderingPixelOrder pixelOrder
			);

			virtual void setup();

			RenderingPixelWriting getPixelWriting() const;
			RenderingPixelOrder getPixelOrder() const;
			ColorModel getColorModel() const;
			const Size& getResolution() const;

			RenderingOrientation getOrientation() const;
			void setOrientation(RenderingOrientation orientation);

			Point orientPoint(const Point& point);

		protected:
			RenderingPixelWriting _pixelWriting;
			RenderingPixelOrder _pixelOrder;
			ColorModel _colorModel;
			const Size _defaultResolution;
			Size _resolution;
			RenderingOrientation _orientation = RenderingOrientation::Clockwise270;

			virtual void updateFromOrientation();
			virtual void onOrientationChanged();
	};
}