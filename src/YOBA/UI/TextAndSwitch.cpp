#include <YOBA/UI/TextAndSwitch.hpp>

namespace YOBA {
	TextAndSwitch::TextAndSwitch() {
		setOrientation(Orientation::horizontal);
		setGap(10);

		textView.setVerticalAlignment(Alignment::center);
		*this += &textView;

		switch_.setVerticalAlignment(Alignment::center);
		setAutoSize(&switch_);
		*this += &switch_;
	}

	TextAndSwitch::TextAndSwitch(const std::string_view title): TextAndSwitch() {
		textView.setText(title);
	}

	TextAndSwitch::TextAndSwitch(const std::string_view title, const bool value): TextAndSwitch(title) {
		switch_.setActive(value);
	}
}
