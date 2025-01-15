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
	};
}