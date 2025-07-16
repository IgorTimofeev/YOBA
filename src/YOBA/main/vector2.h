#pragma once

#include <cstdint>
#include <cmath>

namespace YOBA {
	template<typename T>
	class Vector2 {
		public:
			Vector2();
			Vector2(T x, T y);

			template<class TOther>
			explicit Vector2(const Vector2<TOther>& other);

			T getX() const;
			void setX(T value);

			T getY() const;
			void setY(T value);

			float getLength() const;
			Vector2 <T> clockwisePerpendicular() const;
			Vector2 counterClockwisePerpendicular() const;
			float getRotationFloat(const Vector2& point) const;
			Vector2 rotate(float angleSin, float angleCos) const;
			Vector2 rotate(float angleInRadians) const;
			Vector2 normalize() const;

			Vector2 operator+(const Vector2& right) const;
			Vector2 operator+(T value) const;
			Vector2& operator+=(const Vector2& right);
			Vector2 operator-(const Vector2& right) const;
			Vector2 operator-(T value) const;
			Vector2 operator-() const;
			Vector2<T>& operator-=(const Vector2& right);
			Vector2 operator*(const Vector2& right) const;
			Vector2 operator*(T factor) const;
			Vector2& operator*=(const Vector2& right);
			Vector2 operator/(const Vector2& right) const;
			Vector2 operator/(T factor) const;
			Vector2& operator/=(const Vector2& right);
			bool operator==(const Vector2 &right) const;
			bool operator!=(const Vector2 &right) const;

		private:
			T x;
			T y;
	};

	template<typename T>
	Vector2<T>::Vector2(T x, T y) : x(x), y(y) {

	}

	template<typename T>
	Vector2<T>::Vector2() : Vector2(0, 0) {

	}

	template<typename T>
	template<typename TOther>
	Vector2<T>::Vector2(const Vector2<TOther>& other) : Vector2(static_cast<T>(other.getX()), static_cast<T>(other.getY())) {

	}

	template<typename T>
	T Vector2<T>::getX() const {
		return x;
	}

	template<typename T>
	void Vector2<T>::setX(T value) {
		x = value;
	}

	template<typename T>
	T Vector2<T>::getY() const {
		return y;
	}

	template<typename T>
	void Vector2<T>::setY(T value) {
		y = value;
	}

	template<typename T>
	Vector2<T> Vector2<T>::clockwisePerpendicular() const {
		return {
			y,
			-x,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::counterClockwisePerpendicular() const {
		return {
			-y,
			x,
		};
	}

	template<typename T>
	float Vector2<T>::getLength() const {
		return std::sqrt(static_cast<float>(x) * static_cast<float>(x) + static_cast<float>(y) * static_cast<float>(y));
	}

	template<typename T>
	Vector2<T> Vector2<T>::normalize() const {
		const auto length = getLength();

		return {
			static_cast<T>(static_cast<float>(x) / length),
			static_cast<T>(static_cast<float>(y) / length)
		};
	}

	template<typename T>
	float Vector2<T>::getRotationFloat(const Vector2& point) const {
		return std::atan2(static_cast<float>(point.y - y), static_cast<float>(point.x - x));
	}

	template<typename T>
	Vector2<T> Vector2<T>::rotate(const float angleSin, const float angleCos) const {
		return {
			static_cast<T>(static_cast<float>(x) * angleCos - static_cast<float>(y) * angleSin),
			static_cast<T>(static_cast<float>(x) * angleSin + static_cast<float>(y) * angleCos)
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::rotate(const float angleInRadians) const {
		return rotate(std::sin(angleInRadians), std::cos(angleInRadians));
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& right) const {
		return {
			x + right.x,
			y + right.y,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(T value) const {
		return {
			x + value,
			y + value,
		};
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator+=(const Vector2& right) {
		x += right.x;
		y += right.y;

		return *this;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& right) const {
		return {
			x - right.x,
			y - right.y,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(T value) const {
		return {
			x - value,
			y - value,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-() const {
		return {
			-x,
			-y
		};
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator-=(const Vector2& right) {
		x -= right.x;
		y -= right.y;

		return *this;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(const Vector2<T>& right) const {
		return {
			x * right.x,
			y * right.y,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(T factor) const {
		return {
			x * factor,
			y * factor,
		};
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator*=(const Vector2& right) {
		x *= right.x;
		y *= right.y;

		return *this;
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator/(const Vector2<T>& right) const {
		return {
			x / right.x,
			y / right.y,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator/(T factor) const {
		return {
			x / factor,
			y / factor,
		};
	}

	template<typename T>
	Vector2<T>& Vector2<T>::operator/=(const Vector2& right) {
		x /= right.x;
		y /= right.y;

		return *this;
	}
	
	template<typename T>
	bool Vector2<T>::operator==(const Vector2& right) const {
		return x == right.x && y == right.y;
	}

	template<typename T>
	bool Vector2<T>::operator!=(const Vector2& right) const {
		return !operator==(right);
	}

	using Vector2F = Vector2<float>;
	using Vector2I = Vector2<int32_t>;
	using Point = Vector2I;
}