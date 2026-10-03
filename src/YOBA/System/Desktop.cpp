#include <YOBA/System.hpp>

#ifdef YOBA_SYSTEM_DESKTOP

#include <cstdio>
#include <thread>

namespace YOBA {
	// -------------------------------- System --------------------------------

	void Timer::delayMs(const uint32_t duration) {
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	}

	uint64_t Timer::getTimeUs() {
		static auto startTime = std::chrono::system_clock::now();

		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startTime).count();
	}

	void Memory::reallocate(uint8_t*& buffer, const size_t length) {
		if (buffer)
			delete[] buffer;

		buffer = new uint8_t[length];
	}
}

#endif