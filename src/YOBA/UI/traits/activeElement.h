#pragma once

#include <functional>

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

				if (_onIsActiveChanged)
					_onIsActiveChanged();

				invalidate();
			}

			void setOnIsActiveChanged(const std::function<void()>& callback) {
				_onIsActiveChanged = callback;
			}

		protected:
			virtual void onIsActiveChanged() {

			}

		private:
			std::function<void()> _onIsActiveChanged = nullptr;

			bool _active = false;
	};
}
