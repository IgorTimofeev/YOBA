#pragma once

#include <YOBA/UI/Dialogs/ColorPickerDialog.hpp>

namespace YOBA {
	ColorPickerDialog::ColorPickerDialog() {
		titleTextView.setText("Choose color");

		// Color palette
		colorPalette.setHeight(130);
		contentStackLayout += &colorPalette;
	}
}
