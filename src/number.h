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
}