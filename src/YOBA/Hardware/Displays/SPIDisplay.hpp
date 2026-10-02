#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_SPI

#include <YOBA/Core.hpp>
#include <YOBA/Hardware/Displays/Display.hpp>
#include <YOBA/Hardware/Displays/DisplayInterface.hpp>
#include <YOBA/Hardware/Displays/SPIDisplayInterface.hpp>

namespace YOBA {
	class SPIDisplay : public virtual Display {
		public:
			~SPIDisplay() override = default;

			void setup(
				SPIDisplayInterface* displayInterface,

				const Size& size,
				const Rotation rotation,
				const PixelOrder pixelOrder,
				const ColorModel colorModel
			);

		protected:
			SPIDisplayInterface* _interface = nullptr;

		private:
			// No one should call this anymore
			using RenderingTarget::setup;
	};
}

#endif