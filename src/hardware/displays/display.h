#pragma once

#include "rendering/renderTarget.h"

namespace yoba::hardware {
	class Display : public RenderTarget {
		public:
			Display(
				const Size& resolution,
				RenderingOrientation orientation,
				ColorModel colorModel,
				RenderingPixelWriting pixelWriting,
				RenderingPixelOrder pixelOrder
			) : RenderTarget(resolution, orientation, colorModel, pixelWriting, pixelOrder) {

			}
	};
}
