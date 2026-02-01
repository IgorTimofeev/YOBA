#pragma once

#include <cstdint>
#include <cmath>
#include "sinAndCos.h"

namespace YOBA {
	template<typename T>
	class Vector3 {
		public:
			Vector3();
			Vector3(T x, T y, T z);

			template<class TOther>
			explicit Vector3(const Vector3<TOther>& other);

			T getX() const;
			void setX(T value);

			T getY() const;
			void setY(T value);

			T getZ() const;
			void setZ(T value);

			T getLength() const;
			Vector3 normalize() const;
			Vector3 min(const Vector3& right);
			Vector3 max(const Vector3& right);

			Vector3 rotateAroundXAxis(const SinAndCos& sinAndCos) const;
			Vector3 rotateAroundXAxis(float angle) const;
			Vector3 rotateAroundYAxis(const SinAndCos& sinAndCos) const;
			Vector3 rotateAroundYAxis(float angle) const;
			Vector3 rotateAroundZAxis(const SinAndCos& sinAndCos) const;
			Vector3 rotateAroundZAxis(float angle) const;

			Vector3 operator+(const Vector3& right) const;
			Vector3& operator+=(const Vector3& right);
			Vector3 operator-(const Vector3& right) const;
			Vector3 operator-() const;
			Vector3& operator-=(const Vector3& right);
			Vector3 operator*(T factor) const;
			Vector3 operator*(const Vector3& factor) const;
			Vector3 operator*=(T factor);
			Vector3& operator*=(const Vector3& right);
			Vector3 operator/(T factor) const;
			Vector3 operator/(const Vector3& factor) const;
			Vector3 operator/=(T factor);
			Vector3& operator/=(const Vector3& right);
			bool operator==(const Vector3 &right) const;
			bool operator!=(const Vector3 &right) const;

		private:
			T x;
			T y;
			T z;
	};

	template<typename T>
	Vector3<T>::Vector3(T x, T y, T z) : x(x), y(y), z(z) {

	}

	template<typename T>
	Vector3<T>::Vector3() : Vector3(0, 0, 0) {

	}

	template<typename T>
	template<typename TOther>
	Vector3<T>::Vector3(const Vector3<TOther>& other) : Vector3(static_cast<T>(other.getX()), static_cast<T>(other.getY()), static_cast<T>(other.getZ())) {

	}

	template<typename T>
	T Vector3<T>::getX() const {
		return x;
	}

	template<typename T>
	void Vector3<T>::setX(T value) {
		x = value;
	}

	template<typename T>
	T Vector3<T>::getY() const {
		return y;
	}

	template<typename T>
	void Vector3<T>::setY(T value) {
		y = value;
	}

	template<typename T>
	T Vector3<T>::getZ() const {
		return z;
	}

	template<typename T>
	void Vector3<T>::setZ(T value) {
		z = value;
	}

	template<typename T>
	T Vector3<T>::getLength() const {
		return static_cast<T>(std::sqrt(
			static_cast<float>(x) * static_cast<float>(x)
			+ static_cast<float>(y) * static_cast<float>(y)
			+ static_cast<float>(z) * static_cast<float>(z)
		));
	}

	template<typename T>
	Vector3<T> Vector3<T>::normalize() const {
		auto length = getLength();

		return {
			x / length,
			y / length,
			z / length
		};
	}

	template <typename T>
	Vector3<T> Vector3<T>::min(const Vector3& right) {
		return {
			std::min(x, right.x),
			std::min(y, right.y),
			std::min(z, right.z)
		};
	}

	template <typename T>
	Vector3<T> Vector3<T>::max(const Vector3& right) {
		return {
			std::max(x, right.x),
			std::max(y, right.y),
			std::max(z, right.z)
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::rotateAroundXAxis(const SinAndCos& sinAndCos) const {
		return {
			static_cast<T>(x),
			static_cast<T>(sinAndCos.getCos() * static_cast<float>(y) - sinAndCos.getSin() * static_cast<float>(z)),
			static_cast<T>(sinAndCos.getSin() * static_cast<float>(y) + sinAndCos.getCos() * static_cast<float>(z))
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::rotateAroundXAxis(const float angle) const {
		return rotateAroundXAxis(SinAndCos(angle));
	}

	template<typename T>
	Vector3<T> Vector3<T>::rotateAroundYAxis(const SinAndCos& sinAndCos) const {
		return {
			static_cast<T>(sinAndCos.getCos() * static_cast<float>(x) + sinAndCos.getSin() * static_cast<float>(z)),
			static_cast<T>(y),
			static_cast<T>(-sinAndCos.getSin() * static_cast<float>(x) + sinAndCos.getCos() * static_cast<float>(z))
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::rotateAroundYAxis(const float angle) const {
		return rotateAroundYAxis(SinAndCos(angle));
	}

	template<typename T>
	Vector3<T> Vector3<T>::rotateAroundZAxis(const SinAndCos& sinAndCos) const {
		return {
			static_cast<T>(sinAndCos.getCos() * static_cast<float>(x) - sinAndCos.getSin() * static_cast<float>(y)),
			static_cast<T>(sinAndCos.getSin() * static_cast<float>(x) + sinAndCos.getCos() * static_cast<float>(y)),
			z
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::rotateAroundZAxis(const float angle) const {
		return rotateAroundZAxis(SinAndCos(angle));
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator+(const Vector3& right) const {
		return {
			x + right.x,
			y + right.y,
			z + right.z,
		};
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator+=(const Vector3& right) {
		x += right.x;
		y += right.y;
		z += right.z;

		return *this;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-(const Vector3& right) const {
		return {
			x - right.x,
			y - right.y,
			z - right.z,
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator-() const {
		return {
			-x,
			-y,
			-z,
		};
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator-=(const Vector3& right) {
		x -= right.x;
		y -= right.y;
		z -= right.z;

		return *this;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(T factor) const {
		return {
			x * factor,
			y * factor,
			z * factor
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*(const Vector3& factor) const {
		return {
			x * factor.x,
			y * factor.y,
			z * factor.z
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator*=(T factor) {
		x *= factor;
		y *= factor;
		z *= factor;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator*=(const Vector3& right) {
		x *= right.x;
		y *= right.y;
		z *= right.z;

		return *this;
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator/(T factor) const {
		return {
			x / factor,
			y / factor,
			z / factor
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator/(const Vector3& factor) const {
		return {
			x / factor.x,
			y / factor.y,
			z / factor.z
		};
	}

	template<typename T>
	Vector3<T> Vector3<T>::operator/=(T factor) {
		x /= factor;
		y /= factor;
		z /= factor;

		return *this;
	}

	template<typename T>
	Vector3<T>& Vector3<T>::operator/=(const Vector3& right) {
		x *= right.x;
		y *= right.y;
		z *= right.z;

		return *this;
	}

	template<typename T>
	bool Vector3<T>::operator==(const Vector3& right) const {
		return
			x == right.x
			&& y == right.y
			&& z == right.z;
	}

	template<typename T>
	bool Vector3<T>::operator!=(const Vector3& right) const {
		return !operator==(right);
	}

	using Vector3F = Vector3<float>;
}