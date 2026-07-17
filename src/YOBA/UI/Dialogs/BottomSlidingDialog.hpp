#pragma once

#include <YOBA/UI/Dialogs/Dialog.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/UI/ScrollView.hpp>
#include <YOBA/UI/TextView.hpp>
#include <YOBA/UI/Shapes/RectangularShape.hpp>
#include <YOBA/UI/Transforms/MarginTransform.hpp>

namespace YOBA {
	class BottomSheetDialog : public RelativeStackLayout {
		public:
			BottomSheetDialog();

			DialogOverlayShape overlayShape {};

			Layout backgroundAndContentLayout {};
			RectangularShape backgroundShape {};
	};

	class StackLayoutBottomSheetDialog : public BottomSheetDialog {
		public:
			StackLayoutBottomSheetDialog();

			ScrollView contentScrollView {};

			MarginTransform contentStackLayoutMarginTransform {};
			StackLayout contentStackLayout {};
	};

	class TitleStackLayoutBottomSheetDialog : public StackLayoutBottomSheetDialog {
		public:
			TitleStackLayoutBottomSheetDialog();

			TextView titleTextView {};
	};
}