#include <YOBA/UI/Dialogs/BottomSlidingDialog.hpp>

namespace YOBA {
	BottomSheetDialog::BottomSheetDialog() {
		// Overlay
		*this += &overlayShape;

		// Content and background
		setAutoSize(&backgroundAndContentLayout);
		*this += &backgroundAndContentLayout;

		// Background
		backgroundAndContentLayout += &backgroundShape;
	}

	StackLayoutBottomSheetDialog::StackLayoutBottomSheetDialog() {
		// Scroll view
		backgroundAndContentLayout += &contentScrollView;

		// Content stack layout
		contentStackLayout.setGap(10);

		contentStackLayoutMarginTransform.setMargin({ 15 });
		contentStackLayout.setLayoutTransform(&contentStackLayoutMarginTransform);

		contentScrollView += &contentStackLayout;
	}

	TitleStackLayoutBottomSheetDialog::TitleStackLayoutBottomSheetDialog() {
		contentStackLayout += &titleTextView;
	}
}
