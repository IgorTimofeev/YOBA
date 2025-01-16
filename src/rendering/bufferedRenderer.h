#pragma once

#include "renderer.h"

namespace yoba {
	class BufferedRenderer : public Renderer {
		public:
			explicit BufferedRenderer(RenderTarget* renderTarget);

			void setup() override;

		protected:
			virtual size_t getRequiredBufferLength() = 0;
			virtual void allocate();

			uint8_t* getBuffer() const;
			size_t getBufferLength() const;

		private:
			uint8_t* _buffer = nullptr;
			size_t _bufferLength = 0;
	};
}