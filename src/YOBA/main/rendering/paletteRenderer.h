#pragma once

#include <cstdlib>
#include <cstring>
#include "YOBA/main/rendering/transactionalPixelBufferRenderer.h"

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#endif

namespace YOBA {
	template<typename TIndex, typename TValue>
	class PaletteRenderer : public TransactionalPixelBufferRenderer {
		public:
			explicit PaletteRenderer(TIndex paletteColorCount);

			uint8_t* getPaletteBuffer() const;
			size_t getPaletteBufferLength() const;

			virtual TIndex getPaletteIndex(const Color* color);

			TValue getPaletteValue(TIndex index);
			void setPaletteValue(TIndex index, TValue value);

			void setPaletteColor(TIndex index, const Rgb888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);
			void setPaletteColors(std::initializer_list<Rgb888Color> colors);

			TIndex getPaletteColorCount() const;
			uint8_t* getPalette() const;

		protected:
			void updateFromTarget() override;

			virtual size_t computePaletteBufferLength() const = 0;

		private:
			uint8_t* _paletteBuffer = nullptr;
			size_t _paletteBufferLength;

			uint8_t* _palette = nullptr;
			TIndex _paletteColorCount;

			void reallocatePaletteBuffer();
			void reallocatePalette();
	};

	template<typename TIndex, typename TValue>
	PaletteRenderer<TIndex, TValue>::PaletteRenderer(TIndex paletteColorCount) : _paletteColorCount(paletteColorCount) {

	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::updateFromTarget() {
		TransactionalPixelBufferRenderer::updateFromTarget();

		reallocatePaletteBuffer();
		reallocatePalette();
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::reallocatePaletteBuffer() {
		if (!getTarget())
			return;

		_paletteBufferLength = computePaletteBufferLength();

		#ifdef ESP_PLATFORM
			if (_paletteBuffer)
				heap_caps_free(_paletteBuffer);

			_paletteBuffer = reinterpret_cast<uint8_t*>(heap_caps_malloc(_paletteBufferLength, MALLOC_CAP_DMA));
			assert(_paletteBuffer != nullptr);
		#else
			delete _paletteBuffer;
			_paletteBuffer = new uint8_t[_paletteBufferLength];
		#endif
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::reallocatePalette() {
		if (!getTarget())
			return;

		size_t paletteLength = _paletteColorCount * Color::getBytesPerModel(getTarget()->getColorModel());

		#ifdef ESP_PLATFORM
			if (_palette)
				heap_caps_free(_palette);

			_palette = reinterpret_cast<uint8_t*>(heap_caps_malloc(paletteLength, MALLOC_CAP_DMA));
			assert(_palette != nullptr);
		#else
			delete _palette;
			_palette = new uint8_t[paletteLength];
		#endif
	}

	template<typename TIndex, typename TValue>
	TValue PaletteRenderer<TIndex, TValue>::getPaletteValue(TIndex index) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				return *(reinterpret_cast<TValue*>(_palette) + index);

			case ColorModel::rgb666:
				return ((uint32_t*) (_palette + index * 3))[0];

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteValue(TIndex index, TValue value) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565: {
				*(reinterpret_cast<TValue*>(_palette) + index) = value;
				break;
			}

			case ColorModel::rgb666: {
				const auto palettePtr = _palette + index * 3;
				const auto valuePtr = (uint8_t*) &value;
				palettePtr[0] = valuePtr[2];
				palettePtr[1] = valuePtr[1];
				palettePtr[2] = valuePtr[0];

				break;
			}

			default:
				_palette[index] = 0;
				break;
		}
	}

	template<typename TIndex, typename TValue>
	TIndex PaletteRenderer<TIndex, TValue>::getPaletteIndex(const Color* color) {
		switch (color->getModel()) {
			case ColorModel::palette:
				return reinterpret_cast<const PaletteColor*>(color)->getIndex();

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColor(TIndex index, const Rgb888Color& color) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::rgb565:
				setPaletteValue(index, color.toRgb565().getValue());
				break;

			case ColorModel::rgb666:
				setPaletteValue(index, color.toRgb666().getValue());
				break;

			default:
				setPaletteValue(index, 0);
				break;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColors(std::initializer_list<Rgb888Color> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColors(std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, Rgb888Color(color));
			index++;
		}
	}

	template<typename TIndex, typename TValue>
	TIndex PaletteRenderer<TIndex, TValue>::getPaletteColorCount() const {
		return _paletteColorCount;
	}

	template<typename TIndex, typename TValue>
	uint8_t* PaletteRenderer<TIndex, TValue>::getPalette() const {
		return _palette;
	}

	template<typename TIndex, typename TValue>
	uint8_t* PaletteRenderer<TIndex, TValue>::getPaletteBuffer() const {
		return _paletteBuffer;
	}

	template<typename TIndex, typename TValue>
	size_t PaletteRenderer<TIndex, TValue>::getPaletteBufferLength() const {
		return _paletteBufferLength;
	}
}