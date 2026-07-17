#pragma once

#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/UI/TextView.hpp>
#include <YOBA/UI/Switch.hpp>

namespace YOBA {
	class TextAndSwitch : public RelativeStackLayout {
		public:
			TextAndSwitch();
			TextAndSwitch(const std::string_view title);
			TextAndSwitch(const std::string_view title, const bool value);

			TextView textView {};

			// Fucking keywords...
			Switch switch_ {};
	};
}