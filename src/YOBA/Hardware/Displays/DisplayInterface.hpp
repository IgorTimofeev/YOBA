#pragma once

#include <cstdint>
#include <span>

namespace YOBA {
	class DisplayInterface {
		public:
			virtual ~DisplayInterface() = default;

			void setup(const int8_t RSTPin);

			int8_t getRSTPin() const;
			void setRSTPinState(const bool state) const;
			void toggleRSTPin(uint32_t delayAfterLowMs, uint32_t delayAfterHighMs) const;

			virtual bool write(const uint8_t data) = 0;
			virtual bool write(const std::span<const uint8_t> data) = 0;
			virtual bool writeCommand(const uint8_t command) = 0;

		protected:
			int8_t _RSTPin = -1;

	};
}
