#pragma once

#include <cstdint>
#include <span>

namespace YOBA {
	class DisplayInterface {
		public:
			virtual ~DisplayInterface() = default;

			virtual bool write(const uint8_t data) = 0;
			virtual bool write(const std::span<const uint8_t> data) = 0;
	};
}