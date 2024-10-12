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
				ColorType colorType,
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void setup();

			ColorType getColorType() const;

			const Size& getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);

			Point orientPoint(const Point& point);

		protected:
			ColorType _colorType;
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Landscape270;
//			ColorDepth _colorDepth = ColorDepth::Bit16;

			virtual void updateDataFromOrientation();
			virtual void onOrientationChanged();

			virtual void writePixels(const std::function<uint32_t(size_t pixelIndex)>& pixelGetter) = 0;
	};
}