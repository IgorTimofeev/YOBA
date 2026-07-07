#pragma once

#include <YOBA/Rendering/RenderingTarget.hpp>

namespace YOBA {
	// Maybe will be used in future
	class Display : public virtual RenderingTarget {
		public:
			~Display() override = 0;
	};
}