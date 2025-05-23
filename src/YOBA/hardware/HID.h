#pragma once

namespace YOBA {
	class HID {
		public:
			virtual ~HID() = default;

			virtual void setup() = 0;
			virtual void tick() = 0;
	};
}