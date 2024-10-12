#pragma once

namespace yoba {
	class Application;

	class InputDevice {
		public:
			virtual void setup() = 0;
			virtual void tick(Application* application) = 0;
	};
}