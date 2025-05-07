#pragma once

#include "YOBA/main/vector3.h"
#include <cstdint>
#include <cmath>
#include <numbers>

namespace YOBA {
	class GeographicCoordinates {
		public:
			GeographicCoordinates(float latitude, float longitude, float altitude) : _latitude(latitude), _longitude(longitude), _altitude(altitude) {

			}

			GeographicCoordinates() : GeographicCoordinates(0, 0, 0) {

			}

			explicit GeographicCoordinates(const Vector3F& cartesian, float radius) {
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

			float getLatitude() const {
				return _latitude;
			}

			void setLatitude(float latitude) {
				_latitude = latitude;
			}

			float getLongitude() const {
				return _longitude;
			}

			void setLongitude(float longitude) {
				_longitude = longitude;
			}

			float getAltitude() const {
				return _altitude;
			}

			void setAltitude(float altitude) {
				_altitude = altitude;
			}

		private:
			float _latitude;
			float _longitude;
			float _altitude;
	};
}