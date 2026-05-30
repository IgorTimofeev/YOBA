#pragma once

#include <cstdint>

#include <YOBA/main/vector2.h>

namespace YOBA {
	class Transform {
		public:
			virtual ~Transform() = default;

			virtual Rectangle apply(const Rectangle& bounds) = 0;
	};

	class ScaleTransform : public Transform {
		public:
			ScaleTransform(const Vector2F& scale, const Vector2F& origin = Vector2F(0.5f, 0.5f)) : _scale(scale), _origin(origin) {

			}

			ScaleTransform() : ScaleTransform(Vector2F(1.f, 1.f)) {

			}

			constexpr static float computed = -1.f;

			const Vector2F& getScale() const {
				return _scale;
			}

			void setScale(const Vector2F& scale) {
				_scale = scale;
			}

			void setScale(const float uniformScale) {
				setScale(Vector2F(uniformScale, uniformScale));
			}

			const Vector2F& getOrigin() const {
				return _origin;
			}

			void setOrigin(const Vector2F& origin) {
				_origin = origin;
			}

			Rectangle apply(const Rectangle& bounds) override {
				// Simple as fuck, nothing to do here
				if (_scale.getX() == 1 && _scale.getY() == 1)
					return bounds;

				const auto newWidth = static_cast<uint16_t>(static_cast<float>(bounds.getWidth()) * _scale.getX());
				const auto newHeight = static_cast<uint16_t>(static_cast<float>(bounds.getHeight()) * _scale.getY());

				return {
					bounds.getX()
						- static_cast<int32_t>(
							// DeltaX
							static_cast<float>(static_cast<int32_t>(newWidth) - bounds.getWidth())
							* _origin.getX()
						),
					bounds.getY()
						- static_cast<int32_t>(
							// DeltaY
							static_cast<float>(static_cast<int32_t>(newHeight) - bounds.getHeight())
							* _origin.getY()
						),
					newWidth,
					newHeight,
				};
			}

		private:
			Vector2F _scale;
			Vector2F _origin;
	};
}