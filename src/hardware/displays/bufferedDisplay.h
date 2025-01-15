#pragma once

#include "display.h"

namespace yoba::hardware {
	class BufferedDisplay : public virtual Display {
		public:
			BufferedDisplay();

			void updateFromOrientation() override;

			uint8_t* getPixelBuffer() const;
			size_t getPixelBufferLength() const;
			uint8_t getPixelBufferHeight() const;

			// Driver sends pixel data to screen using tiny sliding window, moving it from
			// top to bottom until complete. This method defines how big window should be.
			// Larger value means more memory use, but less overhead for data transfer.
			//
			// [Fucking important] Make sure screen height is dividable by this.
			//
			// Example: let pixelBufferHeight be 40 px for 320 x 240 px screen.
			// This will allocate 320 * 40 = 25600 pixels * 2 bytes per each = 25 KiB of heap,
			// allowing screen buffer to be flushed in 240 / 40 = 6 equal parts,
			// which more than enough to achieve ~800 FPS on simple scenes on 240 MHz ESP32.
			// Increasing buffer height to 48/60/80/120 px can afford you 10-200 extra FPS,
			// but will eat RAM like a bulimic bitch
			virtual uint8_t getPixelBufferHeightForOrientation() = 0;

			virtual void writePixelBuffer(uint16_t y) = 0;

			void writePixelBuffer(const std::function<void(uint8_t*& destination, size_t& pixelIndex)>& pixelSetter);

		protected:
			uint8_t _pixelBufferHeight = 20;
			size_t _pixelBufferLength = 0;
			uint8_t* _pixelBuffer = nullptr;
	};
}
