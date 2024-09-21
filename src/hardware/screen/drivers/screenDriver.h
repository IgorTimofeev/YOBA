#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "../../../size.h"
#include "point.h"
#include "SPI.h"

namespace yoba {
	enum class ScreenOrientation : uint8_t {
		Portrait0,
		Landscape90,
		Portrait180,
		Landscape270,
	};

	class ScreenDriver {
		public:
			ScreenDriver(
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void begin();

			const Size &getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);
			void rotatePointForOrientation(Point& point);

			virtual void flush(const std::function<uint16_t(size_t pixelIndex)>& flusher) = 0;

		protected:
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Landscape270;

			virtual void updateDataFromOrientation();
			virtual void onOrientationChanged();
	};
}