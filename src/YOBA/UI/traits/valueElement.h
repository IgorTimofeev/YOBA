#pragma once

#include <functional>

#include <YOBA/UI/element.h>

namespace YOBA {
	template<typename TValue>
	class ValueElement : public virtual Element {
		public:
			virtual TValue getValue() const {
				return _value;
			}

			virtual void setValue(const TValue value) {
				if (value == _value)
					return;

				_value = value;

				onValueChanged();

				if (_onValueChanged)
					_onValueChanged();

				invalidate();
			}

			void setOnValueChanged(const std::function<void()>& callback) {
				_onValueChanged = callback;
			}

		protected:
			virtual void onValueChanged() {

			}

		private:
			TValue _value {};

			std::function<void()> _onValueChanged = nullptr;
	};
}
