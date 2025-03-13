#pragma once

namespace yoba::hardware {
	class InvertibleDisplay {
		public:
			virtual void setInverted(bool value) = 0;
	};
}