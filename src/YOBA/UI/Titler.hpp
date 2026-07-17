#pragma once

#include <string_view>

#include <YOBA/UI/StackLayout.hpp>
#include <YOBA/UI/TextView.hpp>

namespace YOBA {
	class Titler : public StackLayout {
		public:
			Titler();
			Titler(Element* element);
			Titler(std::string_view title, Element* element);
			Titler(std::string_view title);

			TextView titleTextView {};
	};
}

