#pragma once

#include <YOBA/UI/Buttons/Button.hpp>
#include <YOBA/Core/Image.hpp>

namespace YOBA {
	class ImageButton : public Button {
		public:
			ImageButton() = default;
			ImageButton(const Image* defaultImage, const Image* activeImage);
			ImageButton(const Image* defaultImage);

			const Image* getDefaultImage() const;
			void setDefaultImage(const Image* const defaultImage);

			const Image* getActiveImage() const;
			void setActiveImage(const Image* const activeImage);

		protected:
			void onRender(Renderer* renderer, const Rectangle& bounds) override;

		private:
			const Image* _defaultImage = nullptr;
			const Image* _activeImage = nullptr;
	};
}
