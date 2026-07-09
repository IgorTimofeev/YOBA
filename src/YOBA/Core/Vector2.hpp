#pragma once

#include <cstdint>
#include <cmath>

namespace YOBA {
	template<typename T>
	class Vector2 {
		public:
			constexpr Vector2(T x, T y) : _x(x), _y(y) {

			}

			constexpr Vector2(T both) : _x(both), _y(both) {

			}

			constexpr Vector2() : _x(0), _y(0) {

			}

			template<typename TOther>
			constexpr Vector2(const Vector2<TOther>& other) : Vector2(static_cast<T>(other.getX()), static_cast<T>(other.getY())) {

			}

			constexpr T getX() const {
				return _x;
			}

			constexpr void setX(T value) {
				_x = value;
			}

			constexpr T getY() const {
				return _y;
			}

			constexpr void setY(T value) {
				_y = value;
			}

			constexpr Vector2 clockwisePerpendicular() const {
				return {
					_y,
					-_x,
				};
			}

			constexpr Vector2 counterClockwisePerpendicular() const {
				return {
					-_y,
					_x,
				};
			}

			constexpr float getLength() const {
				return std::sqrt(static_cast<float>(_x) * static_cast<float>(_x) + static_cast<float>(_y) * static_cast<float>(_y));
			}

			constexpr Vector2 normalize() const {
				const auto length = getLength();

				return {
					static_cast<T>(static_cast<float>(_x) / length),
					static_cast<T>(static_cast<float>(_y) / length)
				};
			}

			constexpr float getRotationFloat(const Vector2& point) const {
				return std::atan2(static_cast<float>(point._y - _y), static_cast<float>(point._x - _x));
			}

			constexpr Vector2 rotate(const float angleSin, const float angleCos) const {
				return {
					static_cast<T>(static_cast<float>(_x) * angleCos - static_cast<float>(_y) * angleSin),
					static_cast<T>(static_cast<float>(_x) * angleSin + static_cast<float>(_y) * angleCos)
				};
			}

			constexpr Vector2 rotate(const float angleInRadians) const {
				return rotate(std::sin(angleInRadians), std::cos(angleInRadians));
			}

			constexpr Vector2 operator+(const Vector2& right) const {
				return {
					_x + right._x,
					_y + right._y,
				};
			}

			constexpr Vector2 operator+(T value) const {
				return {
					_x + value,
					_y + value,
				};
			}

			constexpr Vector2& operator+=(const Vector2& right) {
				_x += right._x;
				_y += right._y;

				return *this;
			}

			constexpr Vector2 operator-(const Vector2& right) const {
				return {
					_x - right._x,
					_y - right._y,
				};
			}

			constexpr Vector2 operator-(T value) const {
				return {
					_x - value,
					_y - value,
				};
			}

			constexpr Vector2 operator-() const {
				return {
					-_x,
					-_y
				};
			}

			constexpr Vector2& operator-=(const Vector2& right) {
				_x -= right._x;
				_y -= right._y;

				return *this;
			}

			constexpr Vector2 operator*(const Vector2& right) const {
				return {
					_x * right._x,
					_y * right._y,
				};
			}

			constexpr Vector2 operator*(T factor) const {
				return {
					_x * factor,
					_y * factor,
				};
			}

			constexpr Vector2& operator*=(const Vector2& right) {
				_x *= right._x;
				_y *= right._y;

				return *this;
			}

			constexpr Vector2 operator/(const Vector2& right) const {
				return {
					_x / right._x,
					_y / right._y,
				};
			}

			constexpr Vector2 operator/(T factor) const {
				return {
					_x / factor,
					_y / factor,
				};
			}

			constexpr Vector2& operator/=(const Vector2& right) {
				_x /= right._x;
				_y /= right._y;

				return *this;
			}
			
			constexpr bool operator==(const Vector2& right) const {
				return _x == right._x && _y == right._y;
			}

			constexpr bool operator!=(const Vector2& right) const {
				return !operator==(right);
			}

		private:
			T _x;
			T _y;
	};

	using Vector2F = Vector2<float>;
	using Vector2I = Vector2<int32_t>;
	// TODO: fuck around if this isn't a sketchy shit idk
	using Point = Vector2I;
}