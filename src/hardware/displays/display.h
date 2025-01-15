#pragma once

#include "rendering/renderTarget.h"

namespace yoba::hardware {
	class Display : public RenderTarget {
		public:
			Display(RenderTargetPixelWriting pixelWriting, RenderTargetPixelOrder pixelOrder, ColorModel colorModel, const Size& resolution, RenderTargetOrientation orientation) : RenderTarget(pixelWriting, pixelOrder, colorModel, resolution, orientation) {

			}
	};
}
