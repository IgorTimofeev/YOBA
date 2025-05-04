#pragma once

#include <numbers>
#include "YOBA/main/vector3.h"

namespace YOBA {
	enum class CameraAxis : uint8_t {
		x,
		y,
		z
	};

	class CameraRotation {
		public:
			CameraRotation(CameraAxis axis, float angle) : _axis(axis), _angle(angle) {

			}

			CameraAxis getAxis() const {
				return _axis;
			}

			void setAxis(CameraAxis axis) {
				_axis = axis;
			}

			float getAngle() const {
				return _angle;
			}

			void setAngle(float value) {
				_angle = value;
			}

		private:
			CameraAxis _axis;
			float _angle;
	};

	class Camera {
		public:
			const Vector3F& getPosition() const {
				return _position;
			}

			void setPosition(const Vector3F& position) {
				_position = position;
			}

			std::array<CameraRotation, 5>& getRotations() {
				return _rotations;
			}

			float getNearPlaneDistance() const {
				return _nearPlaneDistance;
			}

			void setNearPlaneDistance(float value) {
				_nearPlaneDistance = value;
			}

			float getFarPlaneDistance() const {
				return _farPlaneDistance;
			}

			void setFarPlaneDistance(float value) {
				_farPlaneDistance = value;
			}

			float getFOV() const {
				return _FOV;
			}

			void setFOV(float value) {
				_FOV = value;
			}

		private:
			Vector3F _position {};

			std::array<CameraRotation, 5> _rotations{
				CameraRotation(CameraAxis::z, 0),
				CameraRotation(CameraAxis::y, 0),
				CameraRotation(CameraAxis::y, 0),
				CameraRotation(CameraAxis::z, 0),
				CameraRotation(CameraAxis::z, 0)
			};

			// 90 deg by default
			float _FOV = std::numbers::pi_v<float> / 2.f;

			float _nearPlaneDistance = 0.1f;
			float _farPlaneDistance = 1000.f;
	};
}