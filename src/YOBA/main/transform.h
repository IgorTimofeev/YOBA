#pragma once

#include <cstdint>

#include <YOBA/main/vector2.h>

namespace YOBA {
	class Transform {
		public:
			virtual ~Transform() = default;

			virtual Bounds apply(const Bounds& bounds) = 0;
	};

	class ScaleTransform : public Transform {
		public:
			ScaleTransform(const Vector2F& scale, const Vector2F& origin = Vector2F(0.5f, 0.5f)) : _scale(scale), _origin(origin) {

			}

			ScaleTransform(const float scaleX, const float scaleY) : ScaleTransform(Vector2F(scaleX, scaleY)) {

			}

			const Vector2F& getScale() const {
				return _scale;
			}

			void setScale(const Vector2F& scale) {
				_scale = scale;
			}

			const Vector2F& getOrigin() const {
				return _origin;
			}

			void setOrigin(const Vector2F& origin) {
				_origin = origin;
			}

			Bounds apply(const Bounds& bounds) override {
				const auto newWidth = static_cast<uint16_t>(static_cast<float>(bounds.getWidth()) * _scale.getX());
				const auto newHeight = static_cast<uint16_t>(static_cast<float>(bounds.getHeight()) * _scale.getY());

				const auto deltaX = static_cast<int32_t>(newWidth) - bounds.getWidth();
				const auto deltaY = static_cast<int32_t>(newHeight) - bounds.getHeight();

				return {
					bounds.getX() - static_cast<int32_t>(static_cast<float>(deltaX) * _origin.getX()),
					bounds.getY() - static_cast<int32_t>(static_cast<float>(deltaY) * _origin.getY()),
					newWidth,
					newHeight,
				};
			}

		private:
			Vector2F _scale;
			Vector2F _origin;
	};
}