#pragma once

#include <functional>

#include <YOBA/System.hpp>
#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Rectangle.hpp>

namespace YOBA {
	class BufferedRenderer : public virtual Renderer {
		public:
			void setup(
				#ifdef YOBA_SYSTEM_PSRAM
					const bool usePSRAM
				#endif
			)
			#ifndef YOBA_SYSTEM_PSRAM
				override
			#endif
			;

			uint8_t* getPixelBuffer() const;
			size_t getPixelBufferLength() const;

			int32_t getPixelIndex(const int32_t x, const int32_t y) const;
			int32_t getPixelIndex(const Point& point) const;

			uint16_t getFlushingChunkHeight() const;

		protected:
			#ifdef YOBA_SYSTEM_PSRAM
				bool _usePSRAM = false;
			#endif

			uint8_t* _pixelBuffer = nullptr;
			size_t _pixelBufferLength = 0;
			uint16_t _flushingChunkHeight = 0;

			virtual size_t computePixelBufferLength() const = 0;
			void updateFromTarget() override;
			void reallocatePixelBuffer();

			virtual uint16_t computeFlushingChunkHeight() const;

		private:
			using Renderer::setup;
	};
}
