#pragma once

#include "cstdlib"
#include <limits>

namespace yoba {
	template<typename T>
	T clamp(T value, T min, T max) {
		return
			value < min
			? min
			: (
				value > max
				? max
				: value
			);
	}

	template<typename T>
	T round(T value, uint8_t decimalPlaces) {
		const double factor = std::pow(10.0, decimalPlaces);
		return std::round(value * factor) / factor;
	}

	template<typename T>
	T interpolate(T first, T second, float position) {
		return (T) ((float) first + (float) (second - first) * position);
	}

	template<typename T>
	int getDigitCount(T number) {
		if (number < 0)
			number = -number;

		return
			(number < 10 ? 1 :
			(number < 100 ? 2 :
			(number < 1000 ? 3 :
			(number < 10000 ? 4 :
			(number < 100000 ? 5 :
			(number < 1000000 ? 6 :
			(number < 10000000 ? 7 :
			(number < 100000000 ? 8 :
			(number < 1000000000 ? 9 :
			10)))))))));
	}

	template<typename TValue, typename TAddend>
	TValue addSaturating(TValue value, TAddend addend) {
		if (addend >= 0) {
			const auto limit = std::numeric_limits<TValue>::max();

			if ((limit - value) > addend) {
				value += addend;
			}
			else {
				value = limit;
			}
		}
		else {
			const auto limit = std::numeric_limits<TValue>::min();

			if ((value - limit) > -addend) {
				value += addend;
			}
			else {
				value = limit;
			}
		}

		return value;
	}
}