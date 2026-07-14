#include <YOBA/System.hpp>

#if defined(YOBA_SYSTEM_DESKTOP)

#include <cstdio>
#include <chrono>
#include <thread>

namespace YOBA::system {
	// -------------------------------- System --------------------------------

	void delayMs(const uint32_t duration) {
		std::this_thread::sleep_for(std::chrono::milliseconds(duration));
	}

	static auto startTime = std::chrono::system_clock::now();

	uint64_t getTimeUs() {
		return std::chrono::duration_cast<std::chrono::microseconds>(std::chrono::system_clock::now() - startTime).count();
	}

	void reallocate(uint8_t*& buffer, const size_t length) {
		if (buffer)
			delete[] buffer;

		buffer = new uint8_t[length];
	}
}

#endif