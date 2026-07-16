#pragma once

#include <YOBA/UI/Dialogs/BottomSlidingDialog.hpp>

namespace YOBA {
	BottomSheetDialog::BottomSheetDialog() {
		// Overlay
		*this += &overlayShape;

		// Content and background
		setAutoSize(&contentAndBackgroundLayout);
		*this += &contentAndBackgroundLayout;

		// Background
		contentAndBackgroundLayout += &backgroundRectangle;
	}

	StackBottomSheetDialog::StackBottomSheetDialog() {
		// Content layout
		contentStackLayout.setGap(10);

		contentLayoutMarginTransform.setMargin({ 15 });
		contentStackLayout.setLayoutTransform(&contentLayoutMarginTransform);

		contentAndBackgroundLayout += &contentStackLayout;
	}

	TitleStackLayoutBottomSheetDialog::TitleStackLayoutBottomSheetDialog() {
		contentStackLayout += &titleTextView;
	}
}
