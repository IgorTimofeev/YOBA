#pragma once

#include <YOBA/rendering/renderingTarget.h>

namespace YOBA {
	// Maybe will be used in future
	class Display : public virtual RenderingTarget {
		public:
			~Display() override = 0;
	};
}