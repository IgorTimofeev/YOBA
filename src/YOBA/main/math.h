#pragma once

#include "cstdlib"
#include <limits>
#include <numbers>
#include <cmath>

namespace YOBA {
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
	T normalizeAngleDeg360(T angle) {
		angle = std::fmod(angle, 360);
		
		if (angle < 0)
			angle += 360;
		
		return angle;
	}
	
	template<typename T>
	T normalizeAngleDeg180(T angle) {
		angle = std::fmod(angle + 180, 360);
		
		if (angle < 0)
			angle += 360;
		
		return angle - 180;
	}
	
	template<typename T>
	T normalizeAngleRadPi(T angle) {
		const auto twoPi = 2 * std::numbers::pi_v<T>;
		
		angle = std::fmod(angle + std::numbers::pi_v<T>, twoPi);
		
		if (angle < 0)
			angle += twoPi;
		
		return angle - std::numbers::pi_v<T>;
	}
	
	template<typename T>
	T interpolate(const T first, const T second, const T factor) {
		return first + (second - first) * factor;
	}
	
	template<typename T>
	T random(T min, T max) {
		return rand() % (max - min + 1) + min;
	}
	
	template<typename T>
	constexpr uint8_t getDigitCount(T number) {
		if (number < 0)
			number = -number;

		if (number < 10) return 1;
		else if (number < 100) return 2;
		else if (number < 1000) return 3;
		else if (number < 10000) return 4;
		else if (number < 100000) return 5;
		else if (number < 1000000) return 6;
		else if (number < 10000000) return 7;
		else if (number < 100000000) return 8;
		else if (number < 1000000000) return 9;
		else if (number < 10000000000ULL) return 10;
		else if (number < 100000000000ULL) return 11;
		else if (number < 1000000000000ULL) return 12;
		else if (number < 10000000000000ULL) return 13;
		else if (number < 100000000000000ULL) return 14;
		else if (number < 1000000000000000ULL) return 15;
		else if (number < 10000000000000000ULL) return 16;
		else if (number < 100000000000000000ULL) return 17;
		else if (number < 1000000000000000000ULL) return 18;
		else return 19;
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