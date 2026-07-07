#pragma once

#include <cstdint>

namespace YOBA {
	class ContrastDisplay {
		public:
			virtual ~ContrastDisplay() = default;

			virtual void setContrast(uint8_t value) = 0;
	};
}