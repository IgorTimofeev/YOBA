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

	uint64_t getTimeUs() {
		using namespace std::chrono;

		static auto startTime = std::chrono::system_clock::now();

		return duration_cast<microseconds>(system_clock::now() - startTime).count();
	}

	void reallocate(uint8_t*& buffer, const size_t length) {
		if (buffer)
			delete[] buffer;

		buffer = new uint8_t[length];
	}
}

#endif