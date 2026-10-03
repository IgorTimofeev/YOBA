#include <YOBA/Hardware/Displays/Display.hpp>

namespace YOBA {
	void Display::setup(
		DisplayInterface* displayInterface,

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
