#pragma once

#include "YOBA/UI/element.h"
#include "YOBA/main/callback.h"

namespace YOBA {
	class CheckedElement : public virtual Element {
		public:
			Callback<> isCheckedChanged;

			bool isChecked() const {
				return _checked;
			}

			void setChecked(bool value) {
				if (value == _checked)
					return;

				_checked = value;

				onIsCheckedChanged();
				isCheckedChanged();

				invalidate();
			}

		protected:
			virtual void onIsCheckedChanged() {

			}

		private:
			bool _checked = false;
	};
}
