#pragma once

#include <cstdint>
#include <driver/spi_master.h>
#include "../../../size.h"
#include "../../../color.h"
#include "../../../point.h"
#include "../../../screenOrientation.h"
#include "SPI.h"

namespace yoba {
	enum class ScreenDriverPixelWritingType : uint8_t {
		Transactional,
		Full
	};

	enum class ScreenDriverPixelAlignment : uint8_t {
		Flat,
		PaginatedVertical
	};

	class ScreenDriver {
		public:
			ScreenDriver(
				ScreenDriverPixelWritingType pixelWritingType,
				ScreenDriverPixelAlignment pixelAlignment,
				ColorModel colorModel,
				const Size& resolution,
				ScreenOrientation orientation
			);

			virtual void setup();

			ScreenDriverPixelWritingType getPixelWritingType() const;
			ScreenDriverPixelAlignment getPixelAlignment() const;
			ColorModel getColorModel() const;
			const Size& getResolution() const;

			ScreenOrientation getOrientation() const;
			void setOrientation(ScreenOrientation orientation);

			Point orientPoint(const Point& point);

		protected:
			ScreenDriverPixelWritingType _pixelWritingType;
			ScreenDriverPixelAlignment _pixelAlignment;
			ColorModel _colorModel;
			const Size _defaultResolution;
			Size _resolution;
			ScreenOrientation _orientation = ScreenOrientation::Clockwise270;

			virtual void updateDataFromOrientation();
			virtual void onOrientationChanged();
	};
}