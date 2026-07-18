#pragma once

#include <cstdlib>
#include <cstring>

#include <YOBA/Rendering/Renderers/BufferedRenderer.hpp>
#include <YOBA/System.hpp>

namespace YOBA {
	template<typename TIndex, typename TValue>
	class IndexedBufferedRenderer : public BufferedRenderer {
		public:
			explicit IndexedBufferedRenderer(TIndex paletteColorCount);

			uint8_t* getPaletteIndicesBuffer() const;
			size_t getPaletteIndicesBufferLength() const;

			virtual TIndex getPaletteIndex(const Color* color);

			TValue getPaletteValue(TIndex index);
			void setPaletteValue(TIndex index, TValue value);

			void setPaletteColor(TIndex index, const RGB888Color& color);
			void setPaletteColors(std::initializer_list<uint32_t> colors);
			void setPaletteColors(std::initializer_list<RGB888Color> colors);

			TIndex getPaletteColorCount() const;
			uint8_t* getPalette() const;

		protected:
			uint8_t* _paletteBuffer = nullptr;
			TIndex _paletteColorCount;

			uint8_t* _paletteIndicesBuffer = nullptr;
			size_t _paletteIndicesBufferLength = 0;

			void updateFromTarget() override;
			virtual size_t computePaletteIndicesBufferLength() const = 0;

		private:
			void reallocatePaletteIndicesBuffer();
			void reallocatePalette();
	};

	template<typename TIndex, typename TValue>
	IndexedBufferedRenderer<TIndex, TValue>::IndexedBufferedRenderer(TIndex paletteColorCount) : _paletteColorCount(paletteColorCount) {

	}

	template<typename TIndex, typename TValue>
	void IndexedBufferedRenderer<TIndex, TValue>::updateFromTarget() {
		BufferedRenderer::updateFromTarget();

		reallocatePaletteIndicesBuffer();
		reallocatePalette();
	}

	template<typename TIndex, typename TValue>
	void IndexedBufferedRenderer<TIndex, TValue>::reallocatePaletteIndicesBuffer() {
		if (!getTarget())
			return;

		_paletteIndicesBufferLength = computePaletteIndicesBufferLength();
		system::reallocate(_paletteIndicesBuffer, _paletteIndicesBufferLength);
	}

	template<typename TIndex, typename TValue>
	void IndexedBufferedRenderer<TIndex, TValue>::reallocatePalette() {
		if (!getTarget())
			return;

		const size_t paletteBufferLength = _paletteColorCount * Color::getBytesPerModel(getTarget()->getColorModel());
		system::reallocate(_paletteBuffer, paletteBufferLength);
	}

	template<typename TIndex, typename TValue>
	TValue IndexedBufferedRenderer<TIndex, TValue>::getPaletteValue(TIndex index) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::RGB565:
				return *(reinterpret_cast<TValue*>(_paletteBuffer) + index);

			case ColorModel::RGB666:
				return *(reinterpret_cast<TValue*>(_paletteBuffer) + index);

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void IndexedBufferedRenderer<TIndex, TValue>::setPaletteValue(TIndex index, TValue value) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::RGB565: {
				reinterpret_cast<TValue*>(_paletteBuffer)[index] = value;
				break;
			}
			case ColorModel::RGB666: {
				reinterpret_cast<TValue*>(_paletteBuffer)[index] = value;
				break;
			}
			default:
				_paletteBuffer[index] = 0;
				break;
		}
	}

	template<typename TIndex, typename TValue>
	TIndex IndexedBufferedRenderer<TIndex, TValue>::getPaletteIndex(const Color* color) {
		switch (color->getModel()) {
			case ColorModel::indexed8:
				return reinterpret_cast<const Indexed8Color*>(color)->getIndex();

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void IndexedBufferedRenderer<TIndex, TValue>::setPaletteColor(TIndex index, const RGB888Color& color) {
		switch (getTarget()->getColorModel()) {
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
	void IndexedBufferedRenderer<TIndex, TValue>::setPaletteColors(const std::initializer_list<RGB888Color> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, color);
			index++;
		}
	}

	template<typename TIndex, typename TValue>
	void IndexedBufferedRenderer<TIndex, TValue>::setPaletteColors(const std::initializer_list<uint32_t> colors) {
		uint16_t index = 0;

		for (const auto& color : colors) {
			setPaletteColor(index, RGB888Color(color));
			index++;
		}
	}

	template<typename TIndex, typename TValue>
	TIndex IndexedBufferedRenderer<TIndex, TValue>::getPaletteColorCount() const {
		return _paletteColorCount;
	}

	template<typename TIndex, typename TValue>
	uint8_t* IndexedBufferedRenderer<TIndex, TValue>::getPalette() const {
		return _paletteBuffer;
	}

	template<typename TIndex, typename TValue>
	uint8_t* IndexedBufferedRenderer<TIndex, TValue>::getPaletteIndicesBuffer() const {
		return _paletteIndicesBuffer;
	}

	template<typename TIndex, typename TValue>
	size_t IndexedBufferedRenderer<TIndex, TValue>::getPaletteIndicesBufferLength() const {
		return _paletteIndicesBufferLength;
	}
}