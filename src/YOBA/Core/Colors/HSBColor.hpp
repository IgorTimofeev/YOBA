#pragma once

#include <YOBA/Core/Colors/Color.hpp>

namespace YOBA {
	class RGB888Color;

	class HSBColor : public Color {
		public:
			constexpr HSBColor(const float h, const float s, const float b) : Color(ColorModel::HSB), _h(h), _s(s), _b(b) {

			}

			constexpr float getH() const {
				return _h;
			}

			constexpr void setH(const float h) {
				_h = h;
			}

			constexpr float getS() const {
				return _s;
			}

			constexpr void setS(const float s) {
				_s = s;
			}

			constexpr float getB() const {
				return _b;
			}

			constexpr void setB(const float b) {
				_b = b;
			}

			constexpr RGB888Color toRGB888() const;

		private:
			float _h = 0;
			float _s = 0;
			float _b = 0;
	};
}