#include <YOBA/system.h>

#ifdef YOBA_SYSTEM_WINDOWS

namespace YOBA::system {
	void sleep(uint32_t milliseconds) {
		winapi.sleep();
	}

	uint32_t getTime() {
		return winapi.getTime();
	}
}

#endif