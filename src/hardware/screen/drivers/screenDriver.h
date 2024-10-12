#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "../../../size.h"
#include "../../../screenOrientation.h"
#include "point.h"
#include "SPI.h"

namespace yoba {
	class ScreenDriver {
		public:
			ScreenDriver(
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void setup();

			const Size& getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);

			Point orientPoint(const Point& point);

		protected:
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Landscape270;

			virtual void updateDataFromOrientation();
			virtual void onOrientationChanged();
	};

	template<typename TColor>
	class WritableScreenDriver : public ScreenDriver {
		public:
			WritableScreenDriver(const Size& resolution, ScreenOrientation orientation) : ScreenDriver(resolution, orientation) {

			}

			virtual void writePixels(const std::function<TColor(size_t pixelIndex)>& colorGetter) = 0;
	};
}