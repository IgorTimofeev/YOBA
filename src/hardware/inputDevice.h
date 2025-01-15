#pragma once

namespace yoba::ui {
	class Application;
}

namespace yoba::hardware {
	class InputDevice {
		public:
			virtual void setup() = 0;
			virtual void tick(ui::Application* application) = 0;
	};
}