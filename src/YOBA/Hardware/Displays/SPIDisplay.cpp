#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <YOBA/Hardware/Displays/SPIDisplay.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	void SPIDisplay::setup(
		SPIDisplayInterface* displayInterface,

		const Size& size,
		const Rotation rotation,
		const PixelOrder pixelOrder,
		const ColorModel colorModel
	) {
		_interface = displayInterface;

		RenderingTarget::setup(
			size,
			rotation,
			pixelOrder,
			colorModel
		);
	}
}

#endif