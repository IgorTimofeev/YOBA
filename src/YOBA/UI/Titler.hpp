#pragma once

#include <string_view>

#include <YOBA/UI/StackLayout.hpp>
#include <YOBA/UI/TextView.hpp>
#include <YOBA/UI/Element.hpp>

namespace YOBA {
	class Titler : public StackLayout {
		public:
			Titler() {
				setGap(4);

				*this += &titleTextView;
			}

			Titler(Element* element) : Titler() {
				*this += element;
			}

			Titler(std::string_view title, Element* element) : Titler(element) {
				titleTextView.setText(title);
			}

			TextView titleTextView {};
	};
}

