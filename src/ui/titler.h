#pragma once

#include "stackLayout.h"
#include "text.h"

namespace yoba::ui {
	class Titler : public StackLayout {
		public:
			Titler() {
				setSpacing(3);

				*this += &_title;
			}

			explicit Titler(const std::wstring_view& title) : Titler()  {
				_title.setText(title);
			}

			Titler(const std::wstring_view& title, Element* element) : Titler(title) {
				*this += element;
			}

			Text& getTitle() {
				return _title;
			}

		private:
			Text _title = Text();
	};
}