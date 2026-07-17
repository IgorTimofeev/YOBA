#include <YOBA/UI/Titler.hpp>

namespace YOBA {
	Titler::Titler() {
		setGap(4);

		*this += &titleTextView;
	}

	Titler::Titler(Element* element): Titler() {
		*this += element;
	}

	Titler::Titler(std::string_view title, Element* element): Titler(element) {
		titleTextView.setText(title);
	}

	Titler::Titler(std::string_view title): Titler() {
		titleTextView.setText(title);
	}
}
