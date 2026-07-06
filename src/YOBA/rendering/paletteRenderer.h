#pragma once

#include <cstdlib>
#include <cstring>

#include <YOBA/rendering/transactionalBufferedRenderer.h>
#include <YOBA/system.h>

namespace YOBA {
	template<typename TIndex, typename TValue>
	class PaletteRenderer : public TransactionalBufferedRenderer {
		public:
			explicit PaletteRenderer(TIndex paletteColorCount);

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
	PaletteRenderer<TIndex, TValue>::PaletteRenderer(TIndex paletteColorCount) : _paletteColorCount(paletteColorCount) {

	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::updateFromTarget() {
		TransactionalBufferedRenderer::updateFromTarget();

		reallocatePaletteIndicesBuffer();
		reallocatePalette();
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::reallocatePaletteIndicesBuffer() {
		if (!getTarget())
			return;

		_paletteIndicesBufferLength = computePaletteIndicesBufferLength();
		system::reallocate(_paletteIndicesBuffer, _paletteIndicesBufferLength);
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::reallocatePalette() {
		if (!getTarget())
			return;

		const size_t paletteBufferLength = _paletteColorCount * Color::getBytesPerModel(getTarget()->getColorModel());
		system::reallocate(_paletteBuffer, paletteBufferLength);
	}

	template<typename TIndex, typename TValue>
	TValue PaletteRenderer<TIndex, TValue>::getPaletteValue(TIndex index) {
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
	void PaletteRenderer<TIndex, TValue>::setPaletteValue(TIndex index, TValue value) {
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
	TIndex PaletteRenderer<TIndex, TValue>::getPaletteIndex(const Color* color) {
		switch (color->getModel()) {
			case ColorModel::palette8Bit:
				return reinterpret_cast<const Bit8PaletteColor*>(color)->getIndex();

			default:
				return 0;
		}
	}

	template<typename TIndex, typename TValue>
	void PaletteRenderer<TIndex, TValue>::setPaletteColor(TIndex index, const RGB888Color& color) {
		switch (getTarget()->getColorModel()) {
			case ColorModel::RGB565:
				setPaletteValue(index, color.toRGB565BE().getValue());
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
		return _paletteBuffer;
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