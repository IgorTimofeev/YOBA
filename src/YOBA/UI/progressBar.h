#pragma once

#include <YOBA/UI/control.h>
#include <YOBA/UI/traits/cornerRadiusElement.h>
#include <YOBA/UI/traits/valueElement.h>

namespace YOBA {
	class ProgressBar : public Control, public CornerRadiusElement, public ValueElement<float> {
		public:
			const Color* getTrackColor() const;
			void setTrackColor(const Color* value);

			const Color* getFillColor() const;
			void setFillColor(const Color* value);

		protected:
			void onRender(Renderer& renderer, const Bounds& bounds) override;

		private:
			const Color* _trackColor = nullptr;
			const Color* _fillColor = nullptr;
	};
}
