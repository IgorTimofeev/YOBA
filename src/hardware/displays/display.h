#pragma once

#include "rendering/renderTarget.h"

namespace yoba::hardware {
	class Display : public RenderTarget {
		public:
			Display(
				const Size& resolution,
				RenderTargetOrientation orientation,
				ColorModel colorModel,
				RenderTargetPixelWriting pixelWriting,
				RenderTargetPixelOrder pixelOrder
			) : RenderTarget(resolution, orientation, colorModel, pixelWriting, pixelOrder) {

			}
	};
}
