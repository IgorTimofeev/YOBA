#pragma once

#include <cstdint>
#include "hardware/screen/buffers/screenBuffer.h"
#include "hardware/touch/drivers/touchDriver.h"
#include "hardware/touch/touchPanel.h"
#include "ui/workspace.h"

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

			Workspace &getWorkspace() {
				return _workspace;
			}

		private:
			Workspace _workspace = Workspace();

			uint32_t _tickInterval = 1000 / 30;
			uint32_t _tickDeadline = 0;

			ScreenBuffer* _screenBuffer;
			TouchPanel _touchPanel;
	};
}