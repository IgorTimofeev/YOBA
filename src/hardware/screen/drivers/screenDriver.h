#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "../../../size.h"
#include "../../../color.h"
#include "../../../point.h"
#include "../../../screenOrientation.h"
#include "SPI.h"

namespace yoba {
	enum class ScreenDriverBufferType : uint8_t {
		Transactional,
		Full
	};

	class ScreenDriver {
		public:
			ScreenDriver(
				ScreenDriverBufferType bufferType,
				ColorModel colorModel,
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void setup();

			ColorModel getColorModel() const;

			const Size& getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);

			ScreenDriverBufferType getBufferType() const;

			Point orientPoint(const Point& point);

		protected:
			ScreenDriverBufferType _bufferType;
			ColorModel _colorModel;
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Clockwise270;

			virtual void updateDataFromOrientation();
			virtual void onOrientationChanged();
	};
}