#pragma once

#include <algorithm>
#include <cmath>

#include <YOBA/UI/Dialogs/ColorPickerDialog.hpp>
#include <YOBA/Core/Text.hpp>

namespace YOBA {
	ColorPickerDialog::ColorPickerDialog() {
		titleTextView.setText("Choose color");

		// Color palette
		colorPalette.setOnSelectedColorChanged([this] {
			toHSBTextFields();
			toHEXTextField();
		});

		contentStackLayout += &colorPalette;

		// Text field row
		contentStackLayout += &textFieldRow;

		// Text fields
		const auto addHSBTextField = [this](Titler& titler, TextField& textField) {
			textField.setKeyboardLayoutOptions(KeyboardLayoutOptions::numeric);

			textField.setOnInput([this](const auto key, const auto text) {
				if (key != Key::enter)
					return;

				fromHSBTextFields();
			});

			textFieldRow += &titler;
		};

		addHSBTextField(hueTitler, hueTextField);
		addHSBTextField(saturationTitler, saturationTextField);
		addHSBTextField(brightnessTitler, brightnessTextField);

		// HEX
		hexTextField.setKeyboardLayoutOptions(KeyboardLayoutOptions::text);

		hexTextField.setOnInput([this](const auto key, const auto text) {
			if (key != Key::enter)
				return;

			const auto uintColor = Text::tryParseHexOr(hexTextField.getText(), 0);

			colorPalette.setSelectedColor(RGB888Color(uintColor).toHSB());
		});

		textFieldRow += &hexTitler;

		// Confirm button
		confirmButton.setText("Confirm");

		confirmButton.setOnClick([this] {
			if (_onConfirm)
				_onConfirm();
		});

		contentStackLayout += &confirmButton;

		toHSBTextFields();
		toHEXTextField();
	}

	const std::function<void()>& ColorPickerDialog::getOnConfirm() const {
		return _onConfirm;
	}

	void ColorPickerDialog::setOnConfirm(const std::function<void()>& value) {
		_onConfirm = value;
	}

	HSBColor ColorPickerDialog::getSelectedColor() const {
		return colorPalette.getSelectedColor();
	}

	void ColorPickerDialog::setSelectedColor(const HSBColor& value) {
		colorPalette.setSelectedColor(value);
	}

	void ColorPickerDialog::toHSBTextFields() {
		const auto& color = colorPalette.getSelectedColor();

		hueTextField.setText(std::to_string(static_cast<uint16_t>(std::round(color.getH() * 360.f))));
		saturationTextField.setText(std::to_string(static_cast<uint16_t>(std::round(color.getS() * 100.f))));
		brightnessTextField.setText(std::to_string(static_cast<uint16_t>(std::round(color.getB() * 100.f))));
	}

	void ColorPickerDialog::fromHSBTextFields() {
		HSBColor color {};
		color.setH(static_cast<float>(std::clamp<int32_t>(Text::tryParseInt32Or(hueTextField.getText(), 0), 0, 360)) / 360.f);
		color.setS(static_cast<float>(std::clamp<int32_t>(Text::tryParseInt32Or(saturationTextField.getText(), 0), 0, 100)) / 100.f);
		color.setB(static_cast<float>(std::clamp<int32_t>(Text::tryParseInt32Or(brightnessTextField.getText(), 0), 0, 100)) / 100.f);

		colorPalette.setSelectedColor(color);
	}

	void ColorPickerDialog::toHEXTextField() {
		const auto uintColor = colorPalette.getSelectedColor().toRGB888().toUint32();

		char text[32];
		std::snprintf(text, sizeof(text), "%06X", uintColor);

		hexTextField.setText(text);
	}
}
