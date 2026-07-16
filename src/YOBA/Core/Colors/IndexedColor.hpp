#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	template<typename TIndex>
	class IndexedColor : public Color {
		public:
			constexpr IndexedColor(TIndex index) : Color(ColorModel::indexed8), _index(index) {

			}

			constexpr IndexedColor() : IndexedColor(0) {

			}

			constexpr TIndex getIndex() const {
				return _index;
			}

			constexpr void setIndex(TIndex value) {
				_index = value;
			}

		private:
			TIndex _index;
	};

	class Indexed8Color : public IndexedColor<uint8_t> {
		public:
			constexpr Indexed8Color(const uint8_t index) : IndexedColor(index) {

			}

			constexpr Indexed8Color() : Indexed8Color(0) {

			}
	};
}