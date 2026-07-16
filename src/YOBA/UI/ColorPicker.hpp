#pragma once

#include <algorithm>
#include <functional>

#include <YOBA/Core/Colors.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Dialogs/ColorPickerDialog.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/FontElement.hpp>
#include <YOBA/UI/Traits/FontScaleElement.hpp>
#include <YOBA/UI/Traits/TextElement.hpp>

namespace YOBA {
	class ColorPicker : public Control, public CornerRadiusElement, public FontElement, public FontScaleElement {
		public:
			const HSBColor& getSelectedColor() const;
			void setSelectedColor(const HSBColor& selectedColor);

			const std::function<ColorPickerDialog*()>& getDialogBuilder() const;
			void setDialogBuilder(const std::function<ColorPickerDialog*()>& value);

			const std::function<void()>& getOnSelectedColorChanged() const;
			void setOnSelectedColorChanged(const std::function<void()>& value);

			void showDialog();
			void hideDialog();

		protected:
			void onEvent(Event* event) override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

			virtual void onSelectedColorChanged();

		private:
			HSBColor _selectedColor {};

			ColorPickerDialog* _dialog = nullptr;

			std::function<ColorPickerDialog*()> _dialogBuilder = nullptr;
			std::function<void()> _onSelectedColorChanged = nullptr;
	};
}
