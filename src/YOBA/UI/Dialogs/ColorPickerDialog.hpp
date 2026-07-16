#pragma once

#include <YOBA/UI/Dialogs/BottomSlidingDialog.hpp>
#include <YOBA/UI/ColorPalette.hpp>

namespace YOBA {
	class ColorPickerDialog : public TitleStackLayoutBottomSheetDialog {
		public:
			ColorPickerDialog();

			ColorPalette colorPalette {};
	};
}