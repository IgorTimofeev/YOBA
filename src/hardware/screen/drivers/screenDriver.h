#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "../../../size.h"
#include "../../../color.h"
#include "../../../point.h"
#include "../../../screenOrientation.h"
#include "SPI.h"

namespace yoba {
	class ScreenDriver {
		public:
			ScreenDriver(
				ColorModel colorModel,
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void setup();

			ColorModel getColorModel() const;

			const Size& getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);

			Point orientPoint(const Point& point);

			virtual void writePixels(const std::function<void(uint8_t*& destination, size_t pixelIndex)>& pixelSetter) = 0;

		protected:
			ColorModel _colorModel;
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Landscape270;

			virtual void updateDataFromOrientation();
			virtual void onOrientationChanged();
	};
}