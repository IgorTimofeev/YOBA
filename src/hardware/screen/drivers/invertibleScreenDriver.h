#pragma once

namespace yoba {
	class InvertibleScreenDriver {
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