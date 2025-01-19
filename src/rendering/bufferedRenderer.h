#pragma once

#include "renderer.h"

namespace yoba {
	class BufferedRenderer : public Renderer {
		public:

		protected:
			void onTargetChanged() override;

			virtual size_t getRequiredBufferLength() = 0;

			uint8_t* getBuffer() const;
			size_t getBufferLength() const;

		private:
			uint8_t* _buffer = nullptr;
			size_t _bufferLength = 0;
	};
}