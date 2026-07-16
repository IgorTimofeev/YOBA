#pragma once

#include <YOBA/UI/Dialogs/Dialog.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/UI/TextView.hpp>
#include <YOBA/UI/Shapes/RectangularShape.hpp>
#include <YOBA/UI/Transforms/MarginTransform.hpp>

namespace YOBA {
	class BottomSheetDialog : public RelativeStackLayout {
		public:
			BottomSheetDialog();

			DialogOverlayShape overlayShape {};

			Layout contentAndBackgroundLayout {};
			RectangularShape backgroundRectangle {};
	};

	class StackBottomSheetDialog : public BottomSheetDialog {
		public:
			StackBottomSheetDialog();

			MarginTransform contentLayoutMarginTransform {};
			StackLayout contentStackLayout {};
	};

	class TitleStackLayoutBottomSheetDialog : public StackBottomSheetDialog {
		public:
			TitleStackLayoutBottomSheetDialog();

			TextView titleTextView {};
	};
}