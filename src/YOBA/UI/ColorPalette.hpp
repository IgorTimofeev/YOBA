#pragma once

#include <algorithm>
#include <functional>

#include <YOBA/Core/Colors.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>

namespace YOBA {
	class PaletteSelector : public Control {
		public:
			const Color* getHandleColor() const {
				return _handleColor;
			}

			void setHandleColor(const Color* value) {
				_handleColor = value;

				invalidateRender();
			}

			const std::function<void()>& getOnValueChanged() const {
				return _onValueChanged;
			}

			void setOnValueChanged(const std::function<void()>& onValueChanged) {
				_onValueChanged = onValueChanged;
			}

		protected:
			void onEvent(Event* event) override {
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

			virtual void onPointerEvent(const Point& position) = 0;

			void invokeOnValueChanged() const {
				if (_onValueChanged)
					_onValueChanged();
			}

		private:
			const Color* _handleColor = nullptr;

			std::function<void()> _onValueChanged = nullptr;
	};

	class ColorPaletteSBSelector : public PaletteSelector {
		public:
			ColorPaletteSBSelector() {

			}

			float getHue() const {
				return _hue;
			}

			void setHue(const float hue) {
				_hue = hue;
			}

			float getSelectedSaturation() const {
				return _selectedSaturation;
			}

			void setSelectedSaturation(const float value) {
				_selectedSaturation = std::clamp<float>(value, 0.0f, 1.0f);

				invokeOnValueChanged();

				invalidate();
			}

			float getSelectedBrightness() const {
				return _selectedBrightness;
			}

			void setSelectedBrightness(const float value) {
				_selectedBrightness = std::clamp<float>(value, 0.0f, 1.0f);

				invokeOnValueChanged();

				invalidate();
			}

		protected:
			void onPointerEvent(const Point& position) override {
				const auto& bounds = getLayoutBounds();

				setSelectedSaturation(static_cast<float>(position.getX() - bounds.getX()) / static_cast<float>(bounds.getWidth()));
				setSelectedBrightness(static_cast<float>(position.getY() - bounds.getY()) / static_cast<float>(bounds.getHeight()));
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				// Palette
				{
					const auto sStep = 1.0f / static_cast<float>(bounds.getWidth()) * static_cast<float>(_pixelSize);
					const auto bStep = 1.0f / static_cast<float>(bounds.getHeight()) * static_cast<float>(_pixelSize);

					HSBColor hsbColor { _hue, 0, 1 };

					const auto renderPixel = [this, renderer, &bounds](const int32_t x, const int32_t y, const Color* color) noexcept -> void {
						renderer->fillRectangle(
							Rectangle(bounds.getX() + x, bounds.getY() + y, _pixelSize, _pixelSize),
							color
						);
					};

					for (uint16_t y = 0; y < bounds.getHeight(); y += _pixelSize) {
						for (uint16_t x = 0; x < bounds.getWidth(); x += _pixelSize) {
							switch (renderer->getTarget()->getColorModel()) {
								case ColorModel::RGB565: {
									const auto color = hsbColor.toRGB565BE();
									renderPixel(x, y, &color);

									break;
								}
								case ColorModel::RGB888: {
									const auto color = hsbColor.toRGB888();
									renderPixel(x, y, &color);

									break;
								}
								case ColorModel::ARGB: {
									const auto color = hsbColor.toARGB();
									renderPixel(x, y, &color);

									break;
								}
								default:
									break;
							}

							hsbColor.setS(hsbColor.getS() + sStep);
						}

						hsbColor.setS(0);
						hsbColor.setB(hsbColor.getB() - bStep);
					}
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
								+ static_cast<int32_t>(_selectedBrightness * static_cast<float>(bounds.getHeight()))
						},
						_handleSize,
						getHandleColor()
					);
				}
			}

		private:
			uint8_t _pixelSize = 4;

			float _hue = 0;
			float _selectedSaturation = 0;
			float _selectedBrightness = 0;

			const uint16_t _handleSize = 5;
	};

	class ColorPaletteHSelector : public PaletteSelector {
		public:
			ColorPaletteHSelector() {

			}

			float getSelectedHue() const {
				return _selectedHue;
			}

			void setSelectedHue(const float value) {
				_selectedHue = std::clamp<float>(value, 0.0f, 1.0f);

				invokeOnValueChanged();

				invalidate();
			}

		protected:
			void onPointerEvent(const Point& position) override {
				const auto& bounds = getLayoutBounds();

				setSelectedHue(static_cast<float>(position.getX() - bounds.getX()) / static_cast<float>(bounds.getWidth()));
			}

			void onRender(Renderer* renderer, const Rectangle& bounds) override {
				// Palette
				{
					const auto hueStep = 1.0f / static_cast<float>(bounds.getWidth()) * static_cast<float>(_pixelSize);

					HSBColor hsbColor { 0, 1, 1 };

					const auto renderLine = [this, renderer, &bounds](const int32_t x, const Color* color) {
						renderer->fillRectangle(
							{ bounds.getX() + x, bounds.getY(), _pixelSize, bounds.getHeight() },
							color
						);
					};

					for (uint16_t x = 0; x < bounds.getWidth(); x += _pixelSize) {
						switch (renderer->getTarget()->getColorModel()) {
							case ColorModel::RGB565: {
								const auto color = hsbColor.toRGB565BE();
								renderLine(x, &color);

								break;
							}
							case ColorModel::RGB888: {
								const auto color = hsbColor.toRGB888();
								renderLine(x, &color);

								break;
							}
							case ColorModel::ARGB: {
								const auto color = hsbColor.toARGB();
								renderLine(x, &color);

								break;
							}
							default:
								break;
						}

						hsbColor.setH(hsbColor.getH() + hueStep);
					}
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

		private:
			uint8_t _pixelSize = 4;

			float _selectedHue = 0;

			uint16_t _handleWidth = 3;
	};

	class ColorPalette : public RelativeStackLayout {
		public:
			ColorPalette() {
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

			const Color* getHSelectorHandleColor() const {
				return _HSelector.getHandleColor();
			}

			void setHSelectorHandleColor(const Color* value) {
				_HSelector.setHandleColor(value);
			}

			const Color* getSBSelectorHandleColor() const {
				return _SBSelector.getHandleColor();
			}

			void setSBSelectorHandleColor(const Color* value) {
				_SBSelector.setHandleColor(value);
			}

			HSBColor getSelectedColor() const {
				return {
					_HSelector.getSelectedHue(),
					_SBSelector.getSelectedSaturation(),
					_SBSelector.getSelectedBrightness()
				};
			}

			void setSelectedColor(const HSBColor& value) {
				_SBSelector.setHue(value.getH());
				_SBSelector.setSelectedSaturation(value.getS());
				_SBSelector.setSelectedBrightness(value.getB());

				_HSelector.setSelectedHue(value.getH());
			}

			const std::function<void()>& getOnValueChanged() const {
				return _onValueChanged;
			}

			void setOnValueChanged(const std::function<void()>& onValueChanged) {
				_onValueChanged = onValueChanged;
			}

		private:
			ColorPaletteSBSelector _SBSelector {};
			ColorPaletteHSelector _HSelector {};

			std::function<void()> _onValueChanged = nullptr;

			void onAnySelectorValueChanged() const {
				if (_onValueChanged)
					_onValueChanged();
			}
	};
}
