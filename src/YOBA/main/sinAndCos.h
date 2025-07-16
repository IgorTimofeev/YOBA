#pragma once

#include <cstdint>
#include <cmath>

namespace YOBA {
	class SinAndCos {
		public:
			explicit SinAndCos() : _sin(0), _cos(0) {

			}

			explicit SinAndCos(const float angle) : _sin(std::sinf(angle)), _cos(std::cosf(angle)) {

			}

			float getSin() const {
				return _sin;
			}

			void setSin(const float sin) {
				_sin = sin;
			}

			float getCos() const {
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