#pragma once

#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_MCU

#include <YOBA/Rendering/Targets/RenderingTarget.hpp>
#include <YOBA/Hardware/Displays/DisplayInterface.hpp>

namespace YOBA {
	class Display : public virtual RenderingTarget {
		public:
			void setup(
				DisplayInterface* displayInterface,

				const Size& size,
				const Rotation rotation,
				const PixelOrder pixelOrder,
				const ColorModel colorModel
			);

		protected:
			DisplayInterface* _interface = nullptr;

		private:
			// No one should call this anymore
			using RenderingTarget::setup;
	};
}

#endif