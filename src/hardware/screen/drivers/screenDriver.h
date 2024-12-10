#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "size.h"
#include "color.h"
#include "vector.h"
#include "SPI.h"

namespace yoba {
	enum class ScreenOrientation : uint8_t {
		Clockwise0,
		Clockwise90,
		Clockwise180,
		Clockwise270
	};

	enum class ScreenPixelAlignment : uint8_t {
		Horizontal,
		Vertical
	};

	enum class ScreenPixelWritingMethod : uint8_t {
		Direct,
		Buffered
	};

	class ScreenDriver {
		public:
			ScreenDriver(
				ScreenPixelWritingMethod pixelWritingMethod,
				ScreenPixelAlignment pixelAlignment,
				ColorModel colorModel,
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void setup();

			ScreenPixelWritingMethod getPixelWritingMethod() const;
			ScreenPixelAlignment getPixelAlignment() const;
			ColorModel getColorModel() const;
			const Size& getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);

			Point orientPoint(const Point& point);

		protected:
			ScreenPixelWritingMethod _pixelWritingMethod;
			ScreenPixelAlignment _pixelAlignment;
			ColorModel _colorModel;
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Clockwise270;

			virtual void updateFromOrientation();
			virtual void onOrientationChanged();
	};
}