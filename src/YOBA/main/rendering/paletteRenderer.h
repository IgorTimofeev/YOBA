#pragma once

#include <cstdlib>
#include <cstring>
#include <YOBA/main/rendering/transactionalPixelBufferRenderer.h>

#ifdef ESP_PLATFORM
	#include <esp_heap_caps.h>
#endif

namespace YOBA {
	template<typename TIndex, typename TValue>
	class PaletteRenderer : public TransactionalPixelBufferRenderer {
		public:
			explicit PaletteRenderer(TIndex paletteColorCount);

			uint8_t* getPaletteIndicesBuffer() const;
			size_t getPaletteIndicesBufferLength() const;

			virtual TIndex getPaletteIndex(const Color& color);

			TValue getPaletteValue(TIndex index);
			void setPaletteValue(TIndex index, TValue value);

			void setPaletteColor(TIndex index, const RGB888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);
			void setPaletteColors(std::initializer_list<RGB888Color> colors);

			TIndex getPaletteColorCount() const;
			uint8_t* getPalette() const;

		protected:
			uint8_t* _paletteIndicesBuffer = nullptr;
			size_t _paletteIndicesBufferLength = 0;

			uint8_t* _palette = nullptr;
			TIndex _paletteColorCount;

			void updateFromTarget() override;
			virtual size_t computePaletteIndicesBufferLength() const = 0;

		private:
			void reallocatePaletteIndicesBuffer();
			void reallocatePalette();
	};

	template<typename TIndex, typename TValue>
	PaletteRenderer<TIndex, TValue>::PaletteRenderer(TIndex paletteColorCount) : _paletteColorCount(paletteColorCount) {

	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::updateFromTarget() {
		TransactionalPixelBufferRenderer::updateFromTarget();

		reallocatePaletteIndicesBuffer();
		reallocatePalette();
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::reallocatePaletteIndicesBuffer() {
		if (!hasTarget())
			return;

		_paletteIndicesBufferLength = computePaletteIndicesBufferLength();

		#ifdef ESP_PLATFORM
			if (_paletteIndicesBuffer)
				heap_caps_free(_paletteIndicesBuffer);

			_paletteIndicesBuffer = static_cast<uint8_t*>(heap_caps_malloc(_paletteIndicesBufferLength, MALLOC_CAP_DMA));
			assert(_paletteIndicesBuffer != nullptr);
		#else
			delete _paletteIndicesBuffer;
			_paletteIndicesBuffer = new uint8_t[_paletteIndicesBufferLength];
		#endif
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::reallocatePalette() {
		if (!hasTarget())
			return;

		const size_t paletteLength = _paletteColorCount * Color::getBytesPerModel(getTarget().getColorModel());

		#ifdef ESP_PLATFORM
			if (_palette)
				heap_caps_free(_palette);

			_palette = static_cast<uint8_t*>(heap_caps_malloc(paletteLength, MALLOC_CAP_DMA));
			assert(_palette != nullptr);
		#else
			delete _palette;
			_palette = new uint8_t[paletteLength];
		#endif
	}

	template<typename TIndex, typename TValue>
	TValue PaletteRenderer<TIndex, TValue>::getPaletteValue(TIndex index) {
		switch (getTarget().getColorModel()) {
			case ColorModel::RGB565:
				return *(reinterpret_cast<TValue*>(_palette) + index);

			case ColorModel::RGB666:
				return *(reinterpret_cast<TValue*>(_palette) + index);

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteValue(TIndex index, TValue value) {
		switch (getTarget().getColorModel()) {
			case ColorModel::RGB565: {
				reinterpret_cast<TValue*>(_palette)[index] = value;
				break;
			}
			case ColorModel::RGB666: {
				reinterpret_cast<TValue*>(_palette)[index] = value;
				break;
			}
			default:
				_palette[index] = 0;
				break;
		}
	}

	template<typename TIndex, typename TValue>
	TIndex PaletteRenderer<TIndex, TValue>::getPaletteIndex(const Color& color) {
		switch (color.getModel()) {
			case ColorModel::palette8Bit:
				return reinterpret_cast<const Bit8PaletteColor&>(color).getIndex();

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColor(TIndex index, const RGB888Color& color) {
		switch (getTarget().getColorModel()) {
			case ColorModel::RGB565:
				setPaletteValue(index, color.toRGB565().getValue());
				break;

			case ColorModel::RGB666:
				setPaletteValue(index, color.toRGB666().getValue());
				break;

			default:
				setPaletteValue(index, 0);
				break;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColors(const std::initializer_list<RGB888Color> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColors(const std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, RGB888Color(color));
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
	uint8_t* PaletteRenderer<TIndex, TValue>::getPaletteIndicesBuffer() const {
		return _paletteIndicesBuffer;
	}

	template<typename TIndex, typename TValue>
	size_t PaletteRenderer<TIndex, TValue>::getPaletteIndicesBufferLength() const {
		return _paletteIndicesBufferLength;
	}
}