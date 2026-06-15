#pragma once

#include <YOBA/rendering/renderTarget.h>

namespace YOBA {
	// Maybe will be used in future
	class Display : public virtual RenderingTarget {
		public:
			~Display() override = 0;
	};
}