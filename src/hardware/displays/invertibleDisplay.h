#pragma once

namespace yoba::hardware {
	class InvertibleDisplay {
		public:
			bool isInverted() const {
				return _inverted;
			}

			virtual void setInverted(bool value) {
				_inverted = value;
			}

		private:
			bool _inverted = false;
	};
}