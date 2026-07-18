#pragma once

#include <functional>

#include <YOBA/UI/Dialogs/BottomSlidingDialog.hpp>
#include <YOBA/UI/HSBColorPalette.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/UI/TextField.hpp>
#include <YOBA/UI/Titler.hpp>
#include <YOBA/UI/Buttons/TextButton.hpp>

namespace YOBA {
	class ColorPickerDialog : public TitleStackLayoutBottomSheetDialog {
		public:
			ColorPickerDialog();

			HSBColorPalette colorPalette {};

			RelativeStackLayout textFieldRow {
				Orientation::horizontal,
				10
			};

			TextField hueTextField {};
			Titler hueTitler { "H", &hueTextField };

			TextField saturationTextField {};
			Titler saturationTitler { "S", &saturationTextField };

			TextField brightnessTextField {};
			Titler brightnessTitler { "B", &brightnessTextField };

			TextField hexTextField {};
			Titler hexTitler { "HEX", &hexTextField };

			TextButton confirmButton {};

			const std::function<void()>& getOnConfirm() const;
			void setOnConfirm(const std::function<void()>& value);

			HSBColor getSelectedColor() const;
			void setSelectedColor(const HSBColor& value);

		private:
			void toHSBTextFields();
			void fromHSBTextFields();
			void toHEXTextField();

			std::function<void()> _onConfirm = nullptr;
	};
}