#pragma once

namespace YOBA {
	class HID {
		public:
			virtual ~HID() = default;

			virtual void tick() = 0;
	};
}