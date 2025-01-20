#include "../../system.h"

#ifdef YOBA_SYSTEM_WINDOWS

#include "windows.h"

namespace yoba::system {
	void sleep(uint32_t milliseconds) {
		winapi.sleep();
	}

	uint32_t getTime() {
		return winapi.getTime();
	}
}

#endif