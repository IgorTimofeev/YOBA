#pragma once

#include <cstdint>
#include "main/vector2.h"

namespace yoba::hardware {
	class TouchPoint {
		public:
			const Point& getPosition() const;
			void setPosition(const Point& position);

			bool isDown() const;
			void setDown(bool isDown);

		private:
			Point _position = Point();
			bool _isDown = false;
	};
}
