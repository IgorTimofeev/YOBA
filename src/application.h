#pragma once

#include <cstdint>
#include "hardware/screen/buffers/screenBuffer.h"
#include "hardware/touch/drivers/touchDriver.h"
#include "ui/rootLayout.h"

namespace yoba {
	class Application {
		public:
			Application(
				ScreenBuffer* screenBuffer,
				TouchDriver* touchDriver
			);

			virtual void begin();
			virtual void tick();

			uint32_t getTickInterval() const;
			void setTickInterval(uint32_t tickInterval);

			RootLayout &getRoot() {
				return _root;
			}

		private:
			RootLayout _root = RootLayout();

			uint32_t _tickInterval = 1000 / 30;
			uint32_t _tickDeadline = 0;

			ScreenBuffer* _screenBuffer;
			TouchDriver* _touchDriver;
	};
}