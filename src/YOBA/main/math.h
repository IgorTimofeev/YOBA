#pragma once

#include "cstdlib"
#include <limits>
#include <numbers>
#include <cmath>

namespace YOBA {
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
	T round(T value, const uint8_t decimalPlaces) {
		const float factor = std::pow(10.0, decimalPlaces);
		return static_cast<T>(std::roundf(static_cast<float>(value) * factor) / factor);
	}

	template<typename T>
	constexpr float toRadians(T degrees) {
		return static_cast<float>(degrees) * std::numbers::pi_v<float> / 180.f;
	}

	template<typename T>
	constexpr float toDegrees(T radians) {
		return static_cast<float>(radians) * 180.f / std::numbers::pi_v<float>;
	}

	template<typename T>
	T normalizeAngle360(T x) {
		x = std::fmod(x, 360);

		if (x < 0)
			x += 360;

		return x;
	}

	template<typename T>
	T normalizeAngle180(T x) {
		x = std::fmod(x + 180, 360);

		if (x < 0)
			x += 360;

		return x - 180;
	}

	template<typename T>
	T interpolate(T first, T second, const float position) {
		return static_cast<T>(static_cast<float>(first) + static_cast<float>(second - first) * position);
	}

	template<typename T>
	T random(T min, T max) {
		return rand() % (max - min + 1) + min;
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

			if (limit - value > addend) {
				value += addend;
			}
			else {
				value = limit;
			}
		}
		else {
			const auto limit = std::numeric_limits<TValue>::min();

			if (value - limit > -addend) {
				value += addend;
			}
			else {
				value = limit;
			}
		}

		return value;
	}

	template<typename T>
	T divideCeiling(T a, T b) {
		return 1 + (a - 1) / b;
	}
}