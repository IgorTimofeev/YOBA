#pragma once

#include <YOBA/UI/Dialogs/Dialog.hpp>
#include <YOBA/UI/RelativeStackLayout.hpp>
#include <YOBA/UI/Transforms/MarginTransform.hpp>

namespace YOBA {
	class BottomSheetDialog : public RelativeStackLayout {
		public:
			BottomSheetDialog() {
				// Overlay
				*this += &overlayShape;

				// Content and background
				setAutoSize(&contentAndBackgroundLayout);
				*this += &contentAndBackgroundLayout;

				// Background
				contentAndBackgroundLayout += &backgroundRectangle;
			}

			DialogOverlayShape overlayShape {};

			Layout contentAndBackgroundLayout {};
			RectangularShape backgroundRectangle {};
	};

	class StackBottomSheetDialog : public BottomSheetDialog {
		public:
			StackBottomSheetDialog() {
				// Content layout
				contentStackLayout.setGap(10);

				contentLayoutMarginTransform.setMargin({ 15 });
				contentStackLayout.setLayoutTransform(&contentLayoutMarginTransform);

				contentAndBackgroundLayout += &contentStackLayout;
			}

			MarginTransform contentLayoutMarginTransform {};
			StackLayout contentStackLayout {};
	};

	class TitleStackLayoutBottomSheetDialog : public StackBottomSheetDialog {
		public:
			TitleStackLayoutBottomSheetDialog() {
				contentStackLayout += &titleTextView;
			}

			TextView titleTextView {};
	};
}