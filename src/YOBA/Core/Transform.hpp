#pragma once

#include <cstdint>

#include <YOBA/Core/Vector2.hpp>
#include <YOBA/Core/Margin.hpp>

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

	class TranslateTransform : public Transform {
		public:
			TranslateTransform(const Point& value) : _value(value) {

			}

			TranslateTransform() : _value(Point(0, 0)) {

			}

			const Point& getValue() const {
				return _value;
			}

			void setValue(const Point& value) {
				_value = value;
			}

			Rectangle apply(const Rectangle& bounds) override {
				return {
					bounds.getX() + _value.getX(),
					bounds.getY() + _value.getY(),
					bounds.getWidth(),
					bounds.getHeight()
				};
			}

		private:
			Point _value;
	};

	class MarginTransform : public Transform {
		public:
			MarginTransform(const Margin& value) : _value(value) {

			}

			MarginTransform() : _value(Margin(0)) {

			}

			const Margin& getValue() const {
				return _value;
			}

			void setValue(const Margin& value) {
				_value = value;
			}

			Rectangle apply(const Rectangle& bounds) override {
				return {
					bounds.getX() + _value.getLeft(),
					bounds.getY() + _value.getTop(),
					bounds.getWidth() - _value.getLeft() - _value.getRight(),
					bounds.getHeight() - _value.getTop() - _value.getBottom()
				};
			}

		private:
			Margin _value;
	};
}