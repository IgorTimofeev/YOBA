#pragma once

#include <YOBA/UI/element.h>
#include <YOBA/main/callback.h>

namespace YOBA {
	template<typename TValue>
	class ValueElement : public virtual Element {
		public:
			Callback<> valueChanged {};

			virtual TValue getValue() const {
				return _value;
			}

			virtual void setValue(const TValue value) {
				if (value == _value)
					return;

				_value = value;

				onValueChanged();
				valueChanged();

				invalidate();
			}

		protected:
			virtual void onValueChanged() {

			}

		private:
			TValue _value {};
	};
}
