#pragma once

#include <functional>

#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	class BufferedRenderer : public virtual Renderer {
		public:
			uint8_t* getPixelBuffer() const;
			size_t getPixelBufferLength() const;

			int32_t getPixelIndex(const int32_t x, const int32_t y) const;
			int32_t getPixelIndex(const Point& point) const;

			uint16_t getFlushingChunkHeight() const;

		protected:
			uint8_t* _pixelBuffer = nullptr;
			size_t _pixelBufferLength = 0;
			uint16_t _flushingChunkHeight = 0;

			virtual size_t computePixelBufferLength() const = 0;
			void updateFromTarget() override;
			void reallocatePixelBuffer();

			virtual uint16_t computeFlushingChunkHeight() const;
	};
}
