#pragma once

#include <YOBA/UI/Dialogs/BottomSlidingDialog.hpp>
#include <YOBA/UI/ColorPalette.hpp>

namespace YOBA {
	class ColorPickerDialog : public TitleStackLayoutBottomSheetDialog {
		public:
			ColorPickerDialog() {
				titleTextView.setText("Choose color");

				// Color palette
				colorPalette.setHeight(130);
				contentStackLayout += &colorPalette;
			}

			ColorPalette colorPalette {};
	};
}