#pragma once

#include "YOBA/main/rendering/renderer.h"
#include "YOBA/main/bounds.h"
#include <functional>

namespace YOBA {
	class PixelBufferRenderer : public virtual Renderer {
		public:
			uint8_t* getPixelBuffer() const;
			size_t getPixelBufferLength() const;

			size_t getPixelIndex(uint16_t x, uint16_t y) const;
			size_t getPixelIndex(const Point& point) const;

		protected:
			virtual size_t computePixelBufferLength() const = 0;
			void updateFromTarget() override;
			void reallocatePixelBuffer();

		private:
			uint8_t* _pixelBuffer = nullptr;
			size_t _pixelBufferLength = 0;
	};
}
