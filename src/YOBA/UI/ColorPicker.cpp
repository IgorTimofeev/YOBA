#pragma once

#include <YOBA/UI/ColorPicker.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	const HSBColor& ColorPicker::getSelectedColor() const {
		return _selectedColor;
	}

	void ColorPicker::setSelectedColor(const HSBColor& selectedColor) {
		_selectedColor = selectedColor;

		onSelectedColorChanged();

		if (_onSelectedColorChanged)
			_onSelectedColorChanged();
	}

	const std::function<ColorPickerDialog*()>& ColorPicker::getDialogBuilder() const {
		return _dialogBuilder;
	}

	void ColorPicker::setDialogBuilder(const std::function<ColorPickerDialog*()>& value) {
		_dialogBuilder = value;
	}

	const std::function<void()>& ColorPicker::getOnSelectedColorChanged() const {
		return _onSelectedColorChanged;
	}

	void ColorPicker::setOnSelectedColorChanged(const std::function<void()>& value) {
		_onSelectedColorChanged = value;
	}

	void ColorPicker::onEvent(Event* event) {
		Control::onEvent(event);

		if (event->is<PointerDownEvent>()) {
			setCaptured(true);
		}
		else if (event->is<PointerUpEvent>()) {
			setCaptured(false);

			showDialog();
		}
	}

	void ColorPicker::onRender(Renderer* renderer, const Rectangle& bounds) {
		getSelectedColor().toModel(
			renderer->getTarget()->getColorModel(),
			[&](const Color* color) {
				renderer->fillRectangle(
					bounds,
					getCornerRadius(),
					color
				);
			}
		);
	}

	void ColorPicker::onSelectedColorChanged() {

	}

	void ColorPicker::showDialog() {
		if (_dialog || !_dialogBuilder)
			return;

		_dialog = _dialogBuilder();

		_dialog->colorPalette.setSelectedColor(_selectedColor);

		_dialog->colorPalette.setOnValueChanged([this] {
			setSelectedColor(_dialog->colorPalette.getSelectedColor());
		});

		_dialog->overlayShape.setOnPointerEvent([this] {
			Application::getCurrent()->invokeLater([this] {
				hideDialog();
			});
		});

		*Application::getCurrent() += _dialog;
	}

	void ColorPicker::hideDialog() {
		if (!_dialog)
			return;

		*Application::getCurrent() -= _dialog;

		delete _dialog;

		_dialog = nullptr;
	}
}
