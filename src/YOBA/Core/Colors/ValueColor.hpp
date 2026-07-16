#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	#pragma pack(push, 1)

	template<typename TValue>
	class ValueColor : public Color {
		public:
			constexpr ValueColor(const ColorModel model, TValue value) : Color(model), _value(value) {

			}

			constexpr TValue getValue() const {
				return _value;
			}

			constexpr void setValue(TValue value) {
				this->_value = value;
			}

		protected:
			TValue _value;
	};

	#pragma pack(pop)
}