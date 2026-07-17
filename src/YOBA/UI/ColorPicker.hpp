#pragma once

#include <algorithm>
#include <functional>

#include <YOBA/Core/Colors.hpp>
#include <YOBA/UI/Control.hpp>
#include <YOBA/UI/Dialogs/ColorPickerDialog.hpp>
#include <YOBA/UI/Traits/CornerRadiusElement.hpp>
#include <YOBA/UI/Traits/ActiveElement.hpp>

namespace YOBA {
	class ColorPicker : public Control, public CornerRadiusElement, public ActiveElement {
		public:
			const Color* getDefaultBorerColor() const;
			void setDefaultBorerColor(const Color* value);
			const Color* getActiveBorerColor() const;
			void setActiveBorerColor(const Color* value);

			const RGB888Color& getSelectedColor() const;
			void setSelectedColor(const RGB888Color& selectedColor);

			const std::function<void()>& getOnSelectedColorChanged() const;
			void setOnSelectedColorChanged(const std::function<void()>& value);

			const std::function<ColorPickerDialog*()>& getDialogOpener() const;
			void setDialogOpener(const std::function<ColorPickerDialog*()>& value);

			const std::function<void(ColorPickerDialog*)>& getDialogCloser() const;
			void setDialogCloser(const std::function<void(ColorPickerDialog*)>& dialogCloser);

		protected:
			void onEvent(Event* event) override;
			void onPointerOverChanged() override;
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

			virtual void onSelectedColorChanged();

		private:
			RGB888Color _selectedColor {};

			const Color* _defaultBorerColor = nullptr;
			const Color* _activeBorerColor = nullptr;

			std::function<ColorPickerDialog*()> _dialogOpener = nullptr;
			std::function<void(ColorPickerDialog*)> _dialogCloser = nullptr;

			std::function<void()> _onSelectedColorChanged = nullptr;

			void openDialog();
	};
}
