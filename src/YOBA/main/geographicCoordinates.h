#pragma once

#include <YOBA/main/vector3.h>
#include <cstdint>
#include <cmath>
#include <numbers>

namespace YOBA {
	class GeographicCoordinates {
		public:
			GeographicCoordinates(const float latitude, const float longitude, const float altitude) : _latitude(latitude), _longitude(longitude), _altitude(altitude) {

			}

			GeographicCoordinates() : GeographicCoordinates(0, 0, 0) {

			}

			explicit GeographicCoordinates(const Vector3F& cartesian, const float radius) {
				const float length = cartesian.getLength();

				_latitude = std::asinf(cartesian.getZ() / length);
				_longitude = std::atan2f(cartesian.getY(), cartesian.getX());
				_altitude = length - radius;
			}

			explicit GeographicCoordinates(const Vector3F& cartesian) : GeographicCoordinates(cartesian, equatorialRadiusMeters) {

			}

			constexpr static float equatorialRadiusMeters = 6378137.f;
			constexpr static float equatorialLengthMeters = 2.f * std::numbers::pi_v<float> * equatorialRadiusMeters;
			constexpr static float equatorialRadiansPerMeter = 2.f * std::numbers::pi_v<float> / equatorialLengthMeters;
			constexpr static float equatorialMetersPerRadian = equatorialLengthMeters / (2.f * std::numbers::pi_v<float>);

			Vector3F toCartesian(float radius) const {
				radius += _altitude;

				const float latCos = std::cosf(_latitude);

				return Vector3F(
					radius * latCos * std::cosf(_longitude),
					radius * latCos * std::sinf(_longitude),
					radius * std::sinf(_latitude)
				);
			}

			Vector3F toCartesian() const {
				return toCartesian(equatorialRadiusMeters);
			}

			static float getBearing(const float lat1, const float lon1, const float lat2, const float lon2) {
				const auto lonDelta = lon2 - lon1;
				const auto lat2Cos = std::cosf(lat2);

				return std::atan2f(
					std::sinf(lonDelta) * lat2Cos,
					std::cosf(lat1) * std::sinf(lat2) - std::sinf(lat1) * lat2Cos * std::cosf(lonDelta)
				);
			}

			static float getDistance(const float lat1, const float lon1, const float lat2, const float lon2) {
				const auto latDeltaHalf = (lat2 - lat1) / 2;
				const auto lonDeltaHalf = (lon2 - lon1) / 2;

				const auto a = std::sinf(latDeltaHalf) * std::sinf(latDeltaHalf) + std::sinf(lonDeltaHalf) * std::sinf(lonDeltaHalf) * std::cosf(lat1) * std::cosf(lat2);

				return equatorialRadiusMeters * std::atan2f(std::sqrtf(a), std::sqrtf(1 - a)) * 2;
			}

			float getLatitude() const {
				return _latitude;
			}

			void setLatitude(const float latitude) {
				_latitude = latitude;
			}

			float getLongitude() const {
				return _longitude;
			}

			void setLongitude(const float longitude) {
				_longitude = longitude;
			}

			float getAltitude() const {
				return _altitude;
			}

			void setAltitude(const float altitude) {
				_altitude = altitude;
			}

		private:
			float _latitude;
			float _longitude;
			float _altitude;
	};
}