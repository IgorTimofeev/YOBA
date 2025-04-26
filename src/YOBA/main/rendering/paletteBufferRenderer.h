#pragma once

#include <cstdlib>
#include <cstring>
#include "YOBA/main/rendering/transactionBufferRenderer.h"

namespace YOBA {
	class PaletteBufferRenderer : public TransactionBufferRenderer {
		public:
			explicit PaletteBufferRenderer(uint16_t paletteColorCount);

			uint8_t* getPaletteBuffer() const;
			size_t getPaletteBufferLength() const;

			size_t getPaletteBufferIndex(uint16_t x, uint16_t y) const;
			size_t getPaletteBufferIndex(const Point& point) const;

			virtual uint16_t getPaletteIndex(const Color* color);

			uint32_t getPaletteValue(uint16_t index);
			void setPaletteValue(uint16_t index, uint32_t value);

			void setPaletteColor(uint16_t index, const Rgb888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);
			void setPaletteColors(std::initializer_list<Rgb888Color> colors);

		protected:
			void updateFromTarget() override;

			virtual size_t computePaletteBufferLengthForTarget() = 0;

			void reallocatePaletteBuffer();
			void reallocatePalette();

			uint16_t getPaletteColorCount() const;
			uint8_t* getPalette() const;

		private:
			uint8_t* _paletteBuffer = nullptr;
			size_t _paletteBufferLength;

			uint8_t* _palette = nullptr;
			uint16_t _paletteColorCount;
	};
}