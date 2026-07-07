#pragma once

#include <cstdint>
#include <cmath>

namespace YOBA {
	class SinAndCos {
		public:
			constexpr SinAndCos() : _sin(0), _cos(0) {

			}

			constexpr SinAndCos(const float angleDeg) : _sin(std::sinf(angleDeg)), _cos(std::cosf(angleDeg)) {

			}

			constexpr float getSin() const {
				return _sin;
			}

			void setSin(const float sin) {
				_sin = sin;
			}

			constexpr float getCos() const {
				return _cos;
			}

			void setCos(const float cos) {
				_cos = cos;
			}

		private:
			float _sin;
			float _cos;
	};
}