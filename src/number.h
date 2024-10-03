#pragma once

#include "cstdlib"

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
}