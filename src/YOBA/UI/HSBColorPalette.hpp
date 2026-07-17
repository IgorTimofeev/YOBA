#pragma once

#include <algorithm>
#include <functional>

#include <YOBA/Core/Colors.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/Core/Events/PointerEvent.hpp>

namespace YOBA {
	class HSBColorPaletteSelector : public Control {
		public:
			uint8_t getPixelSize() const;
			void setPixelSize(const uint8_t pixelSize);

			const Color* getHandleColor() const;
			void setHandleColor(const Color* value);

			const std::function<void()>& getOnValueChanged() const;
			void setOnValueChanged(const std::function<void()>& onValueChanged);

		protected:
			void onEvent(Event* event) override;

			virtual void onPointerEvent(const Point& position) = 0;

			void invokeOnValueChanged() const;

		private:
			uint8_t _pixelSize = 4;
			const Color* _handleColor = nullptr;

			std::function<void()> _onValueChanged = nullptr;
	};

	class HSBColorPaletteSBSelector : public HSBColorPaletteSelector {
		public:
			float getHue() const;
			void setHue(const float hue);

			float getSelectedSaturation() const;
			void setSelectedSaturation(const float value);

			float getSelectedBrightness() const;
			void setSelectedBrightness(const float value);

		protected:
			void onPointerEvent(const Point& position) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			float _hue = 0.0f;
			float _selectedSaturation = 1.0f;
			float _selectedBrightness = 1.0f;

			const uint16_t _handleSize = 5;
	};

	class HSBColorPaletteHSelector : public HSBColorPaletteSelector {
		public:
			float getSelectedHue() const;
			void setSelectedHue(const float value);

		protected:
			void onPointerEvent(const Point& position) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			float _selectedHue = 0.0f;
			uint16_t _handleWidth = 3;
	};

	class HSBColorPalette : public RelativeStackLayout {
		public:
			HSBColorPalette();

			const Color* getHandleColor() const;
			void setHandleColor(const Color* value);

			uint8_t getPixelSize() const;
			void setPixelSize(const uint8_t value);

			HSBColor getSelectedColor() const;
			void setSelectedColor(const HSBColor& value);

			const std::function<void()>& getOnSelectedColorChanged() const;
			void setOnSelectedColorChanged(const std::function<void()>& value);

		private:
			HSBColorPaletteSBSelector _SBSelector {};
			HSBColorPaletteHSelector _HSelector {};

			std::function<void()> _onSelectedColorChanged = nullptr;

			void onAnySelectorValueChanged() const;
	};
}
