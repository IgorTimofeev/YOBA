#pragma once

#include <YOBA/UI/element.h>

namespace YOBA {
	class ActiveElement : public virtual Element {
		public:
			bool isActive() const {
				return _active;
			}

			void setActive(const bool value) {
				if (value == _active)
					return;

				_active = value;

				onIsActiveChanged();

				invalidate();
			}

		protected:
			virtual void onIsActiveChanged() {

			}

		private:
			bool _active = false;
	};
}
