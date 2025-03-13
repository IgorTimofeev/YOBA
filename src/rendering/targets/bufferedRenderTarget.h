#pragma once

#include "renderTarget.h"
#include <functional>
#include "../../main.h"

namespace yoba::hardware {
	class BufferedRenderTarget : public virtual RenderTarget {
		public:
			void updateFromRotation() override;

			uint8_t* getBuffer() const;
			size_t getBufferLength() const;
			uint8_t getBufferHeight() const;

			// Driver sends pixel data to screen using tiny sliding window, moving it from
			// top to bottom until complete. This method defines how big this window should be.
			// Larger value means more memory use, but less overhead for data transfer.
			//
			// Example: let bufferHeight be 40 px for 320 x 240 px screen with 16-bit color depth.
			// This will allocate 40 * 320 = 25600 pixels * 2 bytes per each = 25 KiB of heap,
			// allowing screen buffer to be flushed in 240 / 40 = 6 equal parts,
			// which more than enough to achieve ~800 FPS on simple scenes on 240 MHz ESP32.
			// Increasing buffer height to 48/60/80/120 px can afford you 10-200 extra FPS,
			// but will eat RAM like a bulimic bitch.
			//
			// [Fucking important] Make sure screen height is dividable by this.
			virtual uint8_t getBufferHeightForRotation() = 0;

			virtual void flushBuffer(const Bounds& bounds, size_t length) = 0;
			void flushBuffer(uint16_t width, const std::function<void(uint8_t*&, uint32_t&)>& pixelSetter);

		protected:
			uint8_t _bufferHeight = 20;
			size_t _bufferLength = 0;
			uint8_t* _buffer = nullptr;
	};
}
