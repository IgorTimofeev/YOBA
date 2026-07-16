#pragma once

#include <YOBA/UI/Element.hpp>
#include <YOBA/Core/Colors.hpp>

namespace YOBA {
	class TrackColorElement : public virtual Element {
		public:
			const Color* getTrackColor() const {
				return _trackColor;
			}

			void setTrackColor(const Color* value) {
				if (value == _trackColor)
					return;

				_trackColor = value;

				invalidate();
			}

		private:
			const Color* _trackColor = nullptr;
	};
}
