#pragma once

#include <cstdint>

namespace YOBA {
	class InvertibleDisplay {
		public:
			virtual ~InvertibleDisplay() = default;

			virtual void setInverted(bool value) = 0;
	};
}