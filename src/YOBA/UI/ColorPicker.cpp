#pragma once

#include <YOBA/UI/ColorPicker.hpp>
#include <YOBA/UI/Application.hpp>

namespace YOBA {
	const Color* ColorPicker::getDefaultBorerColor() const {
		return _defaultBorerColor;
	}

	void ColorPicker::setDefaultBorerColor(const Color* value) {
		_defaultBorerColor = value;
	}

	const Color* ColorPicker::getActiveBorerColor() const {
		return _activeBorerColor;
	}

	void ColorPicker::setActiveBorerColor(const Color* value) {
		_activeBorerColor = value;
	}

	const RGB888Color& ColorPicker::getSelectedColor() const {
		return _selectedColor;
	}

	void ColorPicker::setSelectedColor(const RGB888Color& selectedColor) {
		_selectedColor = selectedColor;

		onSelectedColorChanged();

		if (_onSelectedColorChanged)
			_onSelectedColorChanged();
	}

	const std::function<ColorPickerDialog*()>& ColorPicker::getDialogOpener() const {
		return _dialogOpener;
	}

	void ColorPicker::setDialogOpener(const std::function<ColorPickerDialog*()>& value) {
		_dialogOpener = value;
	}

	const std::function<void(ColorPickerDialog*)>& ColorPicker::getDialogCloser() const {
		return _dialogCloser;
	}

	void ColorPicker::setDialogCloser(const std::function<void(ColorPickerDialog*)>& dialogCloser) {
		_dialogCloser = dialogCloser;
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
			setActive(true);
			setFocused(true);

			event->setHandled(true);
		}
		else if (event->is<PointerUpEvent>()) {
			setActive(false);
			setFocused(false);

			openDialog();

			event->setHandled(true);
		}
	}

	void ColorPicker::onPointerOverChanged() {
		if (isPointerOver())
			return;

		setActive(false);
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

		const auto borderColor = isActive() ? _activeBorerColor : _defaultBorerColor;

		if (borderColor) {
			renderer->strokeRectangle(bounds, getCornerRadius(), borderColor);
		}
	}

	void ColorPicker::onSelectedColorChanged() {

	}

	void ColorPicker::openDialog() {
		if (!_dialogOpener)
			return;

		const auto dialog = _dialogOpener();

		dialog->setSelectedColor(_selectedColor.toHSB());

		dialog->setOnConfirm([this, dialog] {
			setSelectedColor(dialog->getSelectedColor().toRGB888());

			if (_dialogCloser)
				_dialogCloser(dialog);
		});
	}
}
