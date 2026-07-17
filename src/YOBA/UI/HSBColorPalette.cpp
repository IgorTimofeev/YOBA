#pragma once

#include <YOBA/UI/HSBColorPalette.hpp>

namespace YOBA {
	uint8_t HSBColorPaletteSelector::getPixelSize() const {
		return _pixelSize;
	}

	void HSBColorPaletteSelector::setPixelSize(const uint8_t pixelSize) {
		_pixelSize = pixelSize;
	}

	const Color* HSBColorPaletteSelector::getHandleColor() const {
		return _handleColor;
	}

	void HSBColorPaletteSelector::setHandleColor(const Color* value) {
		_handleColor = value;

		invalidateRender();
	}

	const std::function<void()>& HSBColorPaletteSelector::getOnValueChanged() const {
		return _onValueChanged;
	}

	void HSBColorPaletteSelector::setOnValueChanged(const std::function<void()>& onValueChanged) {
		_onValueChanged = onValueChanged;
	}

	void HSBColorPaletteSelector::onEvent(Event* event) {
		Control::onEvent(event);

		if (const auto pointerDownEvent = event->as<PointerDownEvent>()) {
			setCaptured(true);

			onPointerEvent(pointerDownEvent->getPosition());
		}
		else if (const auto pointerDragEvent = event->as<PointerDragEvent>()) {
			onPointerEvent(pointerDragEvent->getPosition());
		}
		else if (event->is<PointerUpEvent>()) {
			setCaptured(false);
		}
	}

	void HSBColorPaletteSelector::invokeOnValueChanged() const {
		if (_onValueChanged)
			_onValueChanged();
	}

	float HSBColorPaletteSBSelector::getHue() const {
		return _hue;
	}

	void HSBColorPaletteSBSelector::setHue(const float hue) {
		_hue = hue;
	}

	float HSBColorPaletteSBSelector::getSelectedSaturation() const {
		return _selectedSaturation;
	}

	void HSBColorPaletteSBSelector::setSelectedSaturation(float value) {
		value = std::clamp<float>(value, 0.0f, 1.0f);

		if (value == _selectedSaturation)
			return;

		_selectedSaturation = value;

		invokeOnValueChanged();

		invalidate();
	}

	float HSBColorPaletteSBSelector::getSelectedBrightness() const {
		return _selectedBrightness;
	}

	void HSBColorPaletteSBSelector::setSelectedBrightness(float value) {
		value = std::clamp<float>(value, 0.0f, 1.0f);

		if (value == _selectedBrightness)
			return;

		_selectedBrightness = value;

		invokeOnValueChanged();

		invalidate();
	}

	void HSBColorPaletteSBSelector::onPointerEvent(const Point& position) {
		const auto& bounds = getLayoutBounds();

		setSelectedSaturation(static_cast<float>(position.getX() - bounds.getX()) / static_cast<float>(bounds.getWidth()));
		setSelectedBrightness(static_cast<float>(bounds.getHeight() - (position.getY() - bounds.getY())) / static_cast<float>(bounds.getHeight()));
	}

	void HSBColorPaletteSBSelector::onRender(Renderer* renderer, const Rectangle& bounds) {
		// Palette
		{
			const auto oldClip = renderer->pushClip(bounds);

			const auto sStep = 1.0f / static_cast<float>(bounds.getWidth()) * static_cast<float>(getPixelSize());
			const auto bStep = 1.0f / static_cast<float>(bounds.getHeight()) * static_cast<float>(getPixelSize());

			HSBColor hsbColor { _hue, 0, 1 };

			for (uint16_t y = 0; y < bounds.getHeight(); y += getPixelSize()) {
				for (uint16_t x = 0; x < bounds.getWidth(); x += getPixelSize()) {
					hsbColor.toModel(
						renderer->getTarget()->getColorModel(),
						[&](const Color* color) {
							renderer->fillRectangle(
								{ bounds.getX() + x, bounds.getY() + y, getPixelSize(), getPixelSize() },
								color
							);
						}
					);

					hsbColor.setS(hsbColor.getS() + sStep);
				}

				hsbColor.setS(0);
				hsbColor.setB(hsbColor.getB() - bStep);
			}

			renderer->setClip(oldClip);
		}

		// Handle
		{
			if (!getHandleColor())
				return;

			renderer->strokeCircle(
				{
					bounds.getX()
						+ static_cast<int32_t>(_selectedSaturation * static_cast<float>(bounds.getWidth())),

					bounds.getY()
						+ bounds.getHeight()
						- static_cast<int32_t>(_selectedBrightness * static_cast<float>(bounds.getHeight()))
				},
				_handleSize,
				getHandleColor()
			);
		}
	}

	float HSBColorPaletteHSelector::getSelectedHue() const {
		return _selectedHue;
	}

	void HSBColorPaletteHSelector::setSelectedHue(float value) {
		value = std::clamp<float>(value, 0.0f, 1.0f);

		if (value == _selectedHue)
			return;

		_selectedHue = value;

		invokeOnValueChanged();

		invalidate();
	}

	void HSBColorPaletteHSelector::onPointerEvent(const Point& position) {
		const auto& bounds = getLayoutBounds();

		setSelectedHue(static_cast<float>(position.getX() - bounds.getX()) / static_cast<float>(bounds.getWidth()));
	}

	void HSBColorPaletteHSelector::onRender(Renderer* renderer, const Rectangle& bounds) {
		// Palette
		{
			const auto oldClip = renderer->pushClip(bounds);

			const auto hueStep = 1.0f / static_cast<float>(bounds.getWidth()) * static_cast<float>(getPixelSize());

			HSBColor hsbColor { 0, 1, 1 };

			for (uint16_t x = 0; x < bounds.getWidth(); x += getPixelSize()) {
				hsbColor.toModel(
					renderer->getTarget()->getColorModel(),
					[&](const Color* color) {
						renderer->fillRectangle(
							{ bounds.getX() + x, bounds.getY(), getPixelSize(), bounds.getHeight() },
							color
						);
					}
				);

				hsbColor.setH(hsbColor.getH() + hueStep);
			}

			renderer->setClip(oldClip);
		}

		// Handle
		{
			if (!getHandleColor())
				return;

			renderer->fillRectangle(
				{
					bounds.getX()
					+ static_cast<int32_t>(_selectedHue * static_cast<float>(bounds.getWidth()))
					- _handleWidth / 2,
					bounds.getY(),
					_handleWidth,
					bounds.getHeight()
				},
				getHandleColor()
			);
		}
	}

	HSBColorPalette::HSBColorPalette() {
		setGap(10);

		// Saturation & brightness
		_SBSelector.setOnValueChanged([this] {
			onAnySelectorValueChanged();
		});

		*this += &_SBSelector;

		// Hue
		_HSelector.setHeight(20);
		setAutoSize(&_HSelector);

		_HSelector.setOnValueChanged([this] {
			_SBSelector.setHue(_HSelector.getSelectedHue());

			onAnySelectorValueChanged();
		});

		*this += &_HSelector;
	}

	const Color* HSBColorPalette::getHandleColor() const {
		return _HSelector.getHandleColor();
	}

	void HSBColorPalette::setHandleColor(const Color* value) {
		_HSelector.setHandleColor(value);
		_SBSelector.setHandleColor(value);
	}

	uint8_t HSBColorPalette::getPixelSize() const {
		return _HSelector.getPixelSize();
	}

	void HSBColorPalette::setPixelSize(const uint8_t value) {
		_HSelector.setPixelSize(value);
		_SBSelector.setPixelSize(value);
	}

	HSBColor HSBColorPalette::getSelectedColor() const {
		return {
			_SBSelector.getHue(),
			_SBSelector.getSelectedSaturation(),
			_SBSelector.getSelectedBrightness()
		};
	}

	void HSBColorPalette::setSelectedColor(const HSBColor& value) {
		_SBSelector.setHue(value.getH());
		_SBSelector.setSelectedSaturation(value.getS());
		_SBSelector.setSelectedBrightness(value.getB());

		_HSelector.setSelectedHue(value.getH());
	}

	const std::function<void()>& HSBColorPalette::getOnSelectedColorChanged() const {
		return _onSelectedColorChanged;
	}

	void HSBColorPalette::setOnSelectedColorChanged(const std::function<void()>& value) {
		_onSelectedColorChanged = value;
	}

	void HSBColorPalette::onAnySelectorValueChanged() const {
		if (_onSelectedColorChanged)
			_onSelectedColorChanged();
	}
}
