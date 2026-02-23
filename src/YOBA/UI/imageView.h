#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/main/image.h>

namespace YOBA {
	class ImageView : public Control {
		public:
			ImageView() = default;
			ImageView(const Image* image);

			const Image* getImage() const;
			void setImage(const Image* value);

		protected:
			Size onMeasure(const Size& availableSize) override;
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			const Image* _image = nullptr;
	};
}
