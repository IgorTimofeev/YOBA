#pragma once

#include "Arduino.h"
#include <cstdint>

namespace yoba {
	template<typename T>
	class Vector2 {
		public:
			Vector2();
			Vector2(T x, T y);

			template<class TOther>
			explicit Vector2(const Vector2<TOther>& point);

			T getX() const;
			void setX(T value);

			T getY() const;
			void setY(T value);

			T getLength() const;
			Vector2 perpendicular() const;
			float getRotation(const Vector2& point) const;
			Vector2 rotate(float angleSin, float angleCos) const;
			Vector2 rotate(float angleInRadians) const;
			Vector2 normalize() const;

			Vector2 operator+(const Vector2& right) const;
			Vector2 operator-(const Vector2& right) const;
			Vector2 operator*(T factor) const;
			Vector2 operator/(T factor) const;
			bool operator==(const Vector2 &right) const;
			bool operator!=(const Vector2 &right) const;

		private:
			T _x;
			T _y;
	};

	template<typename T>
	Vector2<T>::Vector2(T x, T y) : _x(x), _y(y) {

	}

	template<typename T>
	Vector2<T>::Vector2() : Vector2(0, 0) {

	}

	template<typename T>
	template<typename TOther>
	Vector2<T>::Vector2(const Vector2<TOther>& point) : Vector2((T) point.getX(), (T) point.getY()) {

	}

	template<typename T>
	T Vector2<T>::getX() const {
		return _x;
	}

	template<typename T>
	void Vector2<T>::setX(T value) {
		_x = value;
	}

	template<typename T>
	T Vector2<T>::getY() const {
		return _y;
	}

	template<typename T>
	void Vector2<T>::setY(T value) {
		_y = value;
	}

	template<typename T>
	Vector2<T> Vector2<T>::perpendicular() const {
		return {
			-_y,
			_x,
		};
	}

	template<typename T>
	T Vector2<T>::getLength() const {
		return (T) sqrt((float) _x * (float) _x + (float) _y * (float) _y);
	}

	template<typename T>
	Vector2<T> Vector2<T>::normalize() const {
		auto length = getLength();

		return {
			_x / length,
			_y / length
		};
	}

	template<typename T>
	float Vector2<T>::getRotation(const Vector2& point) const {
		return atan2f((float) (point._y - _y), (float) (point._x - _x));
	}

	template<typename T>
	Vector2<T> Vector2<T>::rotate(float angleSin, float angleCos) const {
		return {
			(T) ((float) _x * angleCos - (float) _y * angleSin),
			(T) ((float) _x * angleSin + (float) _y * angleCos)
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::rotate(float angleInRadians) const {
		return rotate(sin(angleInRadians), cos(angleInRadians));
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator*(T factor) const {
		return {
			_x * factor,
			_y * factor,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator/(T factor) const {
		return {
			_x / factor,
			_y / factor,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator+(const Vector2& right) const {
		return {
			_x + right._x,
			_y + right._y,
		};
	}

	template<typename T>
	Vector2<T> Vector2<T>::operator-(const Vector2& right) const {
		return {
			_x - right._x,
			_y - right._y,
		};
	}

	template<typename T>
	bool Vector2<T>::operator==(const Vector2& right) const {
		return _x == right._x && _y == right._y;
	}

	template<typename T>
	bool Vector2<T>::operator!=(const Vector2& right) const {
		return !operator==(right);
	}

	using Vector2F = Vector2<float>;
	using Vector2I = Vector2<int32_t>;
	using Point = Vector2I;
}