#pragma once

#include <YOBA/main/rendering/renderTarget.h>

namespace YOBA {
	// Maybe will be used in future
	class Display : public virtual RenderTarget {
		public:
			~Display() override = 0;
	};
}