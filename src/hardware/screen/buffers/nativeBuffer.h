#pragma once

#include "screenBuffer.h"
#include "fonts/font.h"
#include "../../../color.h"

namespace yoba {
	template<typename TColor>
	class NativeBuffer : public ScreenBuffer {
		public:
			NativeBuffer(ScreenDriver *driver, const Size& size, ScreenRotation rotation);

			void clear(const Color* color) override;
			void renderPixel(const Point &point, const Color* color) override;
			void renderHorizontalLine(const Point &point, uint16_t length, const Color* color) override;
			void renderVerticalLine(const Point &point, uint16_t length, const Color* color) override;
			void renderFilledRectangle(const Bounds& bounds, const Color* color) override;

		protected:
			virtual TColor getNativeColor(const Color* color) = 0;
			virtual void clearNative(TColor color) = 0;
			virtual void renderPixelNative(const Point &point, TColor color) = 0;
			virtual void renderVerticalLineNative(const Point &point, uint16_t width, TColor color) = 0;
			virtual void renderHorizontalLineNative(const Point &point, uint16_t width, TColor color) = 0;
			virtual void renderFilledRectangleNative(const Bounds& bounds, TColor color) = 0;
	};

	template<typename TColor>
	NativeBuffer<TColor>::NativeBuffer(ScreenDriver *driver, const Size &size, ScreenRotation rotation) : ScreenBuffer(driver, size, rotation) {

	}

	template<typename TColor>
	void NativeBuffer<TColor>::clear(const Color* color) {
		clearNative(getNativeColor(color));
	}

	template<typename TColor>
	void NativeBuffer<TColor>::renderPixel(const Point &point, const Color* color) {
		if (getViewport().intersects(point))
			renderPixelNative(point, getNativeColor(color));
	}

	template<typename TColor>
	void NativeBuffer<TColor>::renderHorizontalLine(const Point &point, uint16_t length, const Color* color) {
		const auto& viewport = getViewport();

		if (
			point.getX() > viewport.getX2()
			|| point.getX() + length < viewport.getX()

			|| point.getY() < viewport.getY()
			|| point.getY() > viewport.getY2()
		)
			return;

		uint16_t x1 = max(point.getX(), viewport.getX());
		uint16_t x2 = min(point.getX() + length - 1, viewport.getX2());
		length = x2 - x1 + 1;

		renderHorizontalLineNative(Point(x1, point.getY()), length, getNativeColor(color));
	}

	template<typename TColor>
	void NativeBuffer<TColor>::renderVerticalLine(const Point &point, uint16_t length, const Color* color) {
		const auto& viewport = getViewport();

		if (
			point.getX() < viewport.getX()
			|| point.getX() > viewport.getX2()

			|| point.getY() > viewport.getY2()
			|| point.getY() + length < viewport.getY()
		)
			return;

		uint16_t y1 = max(point.getY(), viewport.getY());
		uint16_t y2 = min(point.getY() + length - 1, viewport.getY2());
		length = y2 - y1 + 1;

		renderVerticalLineNative(Point(point.getX(), y1), length, getNativeColor(color));
	}

	template<typename TColor>
	void NativeBuffer<TColor>::renderFilledRectangle(const Bounds &bounds, const Color* color) {
		const auto& viewport = getViewport();

		if (viewport.intersects(bounds))
			renderFilledRectangleNative(viewport.getIntersection(bounds), getNativeColor(color));
	}
}