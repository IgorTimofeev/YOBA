#pragma once

#include <cstdlib>
#include <limits>
#include <numbers>
#include <cmath>

namespace YOBA {
	class Math {
		public:
			constexpr static auto pi = std::numbers::pi_v<float>;
			constexpr static auto twoPi = 2.f * pi;
			constexpr static auto halfPi = pi / 2.f;

			constexpr static float round(const float value, const uint8_t decimalPlaces) {
				const float factor = std::pow(10.0, decimalPlaces);
				return std::roundf(value * factor) / factor;
			}
			
			constexpr static float toRadians(const float degrees) {
				return static_cast<float>(degrees) * pi / 180.f;
			}
			
			constexpr static float toDegrees(const float  radians) {
				return static_cast<float>(radians) * 180.f / pi;
			}
			
			constexpr static float normalizeAngleDeg360(float angle) {
				angle = std::fmodf(angle, 360);

				if (angle < 0)
					angle += 360;

				return angle;
			}

			constexpr static float normalizeAngleDeg180(float angle) {
				angle = std::fmodf(angle + 180, 360);

				if (angle < 0)
					angle += 360;

				return angle - 180;
			}

			constexpr static float normalizeAngleRadPi(float angle) {
				angle = std::fmodf(angle + pi, twoPi);

				if (angle < 0)
					angle += twoPi;

				return angle - pi;
			}

			constexpr static float normalizeAngleRad2Pi(float angle) {

				angle = std::fmodf(angle, twoPi);

				if (angle < 0)
					angle += twoPi;

				return angle;
			}

			constexpr static float interpolate(const float first, const float second, const float factor) {
				return first + (second - first) * factor;
			}
			
			template<typename T>
			static T random(T min, T max) {
				return rand() % (max - min + 1) + min;
			}
			
			template<typename T>
			constexpr static uint8_t getDigitCount(T number) {
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
			static TValue addSaturating(TValue value, TAddend addend) {
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
			
			template<std::integral T>
			static T divideCeiling(T dividend, T divisor) {
				return 1 + (dividend - 1) / divisor;
			}

			template<std::integral T>
			static T divideRounding(T dividend, T divisor) {
				const auto offset = (((dividend < 0) == (divisor < 0)) ? divisor : -divisor) / 2;

				return (dividend + offset) / divisor;
			}
	};
}