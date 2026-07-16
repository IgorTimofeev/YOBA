#include <cassert>

#include <YOBA/Rendering/Renderers/Renderer.hpp>
#include <YOBA/Core/Math.hpp>
#include <YOBA/Core/UTF-8.hpp>

namespace YOBA {
	RenderingTarget* Renderer::getTarget() const {
		return _target;
	}

	void Renderer::setTarget(RenderingTarget* value) {
		if (value == _target || (_target && *value == *_target))
			return;

		const auto previousTarget = _target;

		if (previousTarget)
			previousTarget->_renderer = nullptr;

		_target = value;
		_target->_renderer = this;

		resetClip();
		updateFromTarget();
	}

	void Renderer::updateFromTarget() {

	}

	const Rectangle& Renderer::getClip() const {
		return _clip;
	}

	void Renderer::setClip(const Rectangle& bounds) {
		_clip = bounds;
	}

	Rectangle Renderer::pushClip(const Rectangle& bounds) {
		const auto oldClipBounds = _clip;

		if (_clip.intersects(bounds)) {
			_clip = _clip.getIntersection(bounds);
		}
		else {
			_clip = { 0, 0, 0, 0 };
		}

		return oldClipBounds;
	}

	void Renderer::resetClip() {
		_clip.setX(0);
		_clip.setX(0);
		_clip.setSize(_target->getSize());
	}

	// -------------------------------- Native rendering --------------------------------

	void Renderer::clear(const Color* color) {
		clearNative(color);
	}

	void Renderer::putPixel(const Point& position, const Color* color) {
		if (!getClip().contains(position))
			return;

		putPixelNative(position, color);
	}

	void Renderer::strokeHorizontalLine(const Point& position, uint16_t length, const Color* color) {
		const auto& clip = getClip();

		if (
			length == 0
			|| position.getX() > clip.getX2()
			|| position.getX() + length < clip.getX()

			|| position.getY() < clip.getY()
			|| position.getY() > clip.getY2()
			)
			return;

		const uint16_t x1 = std::max(position.getX(), clip.getX());
		const uint16_t x2 = std::min(position.getX() + length - 1, clip.getX2());
		length = x2 - x1 + 1;

		strokeHorizontalLineNative(Point(x1, position.getY()), length, color);
	}

	void Renderer::strokeVerticalLine(const Point& position, uint16_t length, const Color* color) {
		const auto& clip = getClip();

		if (
			length == 0
			|| position.getX() < clip.getX()
			|| position.getX() > clip.getX2()

			|| position.getY() > clip.getY2()
			|| position.getY() + length < clip.getY()
		)
			return;

		const uint16_t y1 = std::max(position.getY(), clip.getY());
		const uint16_t y2 = std::min(position.getY() + length - 1, clip.getY2());
		length = y2 - y1 + 1;

		strokeVerticalLineNative(Point(position.getX(), y1), length, color);
	}

	void Renderer::fillRectangle(const Rectangle& bounds, const Color* color) {
		const auto& clip = getClip();

		if (bounds.haveZeroSize() || !clip.intersects(bounds))
			return;

		const auto& intersection = clip.getIntersection(bounds);

		if (intersection.getWidth() > 1 || intersection.getHeight() > 1) {
			fillRectangleNative(intersection, color);
		}
		else if (intersection.getWidth() == 1) {
			strokeVerticalLine(intersection.getPosition(), intersection.getHeight(), color);
		}
		else if (intersection.getHeight() == 1) {
			strokeHorizontalLine(intersection.getPosition(), intersection.getWidth(), color);
		}
		else {
			putPixelNative(intersection.getPosition(), color);
		}
	}

	// #-#-#-#-
	// -#-#-#-#
	void Renderer::fillChessPatternRectangle(const Rectangle& bounds, const Color* color, const uint8_t filledStep, const uint8_t transparentStep) {
		const auto x2 = bounds.getX2();
		const auto y2 = bounds.getY2();
		auto odd = true;

		if (filledStep == 1 && transparentStep == 1) {
			for (int32_t y = bounds.getY(); y <= y2; y++) {
				for (int32_t x = odd ? bounds.getX() + filledStep : bounds.getX(); x <= x2; x += 2)
					putPixel(Point(x, y), color);

				odd = !odd;
			}
		}
		else {
			const auto totalSize = filledStep + transparentStep;

			for (int32_t y = bounds.getY(); y <= y2; y++) {
				for (int32_t x = odd ? bounds.getX() + filledStep / 2 : bounds.getX(); x <= x2; x += totalSize)
					strokeHorizontalLine(Point(x, y), filledStep, color);

				odd = !odd;
			}
		}
	}

	void Renderer::fillQuad(const Point& topLeft, const Point& topRight, const Point& bottomRight, const Point& bottomLeft, const Color* color) {
		fillTriangle(topLeft, topRight, bottomRight, color);
		fillTriangle(topLeft, bottomLeft, bottomRight, color);
	}

	void Renderer::putImage(const Point& position, const Image* image) {
		if (getClip().intersects(Rectangle(position, image->getSize())))
			putImageNative(position, image);
	}

	// -------------------------------- Non-native rendering --------------------------------

	void Renderer::fillRectangle(const Rectangle& bounds, const uint16_t cornerRadius, const Color* color) {
		if (cornerRadius > 0) {
			// Rect in middle
			fillRectangle(
				Rectangle(
					bounds.getX(),
					bounds.getY() + cornerRadius,
					bounds.getWidth(),
					bounds.getHeight() - cornerRadius - cornerRadius
				),
				color
			);

			// 2 upper corners
			fillUpperRoundedCorners(
				Point(
					bounds.getX() + cornerRadius,
					bounds.getY() + cornerRadius
				),
				cornerRadius,
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);

			// And 2 lower
			fillLowerRoundedCorners(
				Point(
					bounds.getX() + cornerRadius,
					bounds.getY() + bounds.getHeight() - cornerRadius - 1
				),
				cornerRadius,
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);
		}
		else {
			fillRectangle(bounds, color);
		}
	}

	void Renderer::strokeRectangle(const Rectangle& bounds, const Color* color) {
		if (bounds.haveZeroSize())
			return;

		if (bounds.getWidth() > 1 || bounds.getHeight() > 1) {
			strokeHorizontalLine(
				bounds.getTopLeft(),
				bounds.getWidth(),
				color
			);

			strokeHorizontalLine(
				bounds.getBottomLeft(),
				bounds.getWidth(),
				color
			);

			strokeVerticalLine(
				Point(
					bounds.getX(),
					bounds.getY() + 1
				),
				bounds.getHeight() - 2,
				color
			);

			strokeVerticalLine(
				Point(
					bounds.getX2(),
					bounds.getY() + 1
				),
				bounds.getHeight() - 2,
				color
			);
		}
		else {
			putPixel(bounds.getPosition(), color);
		}
	}

	void Renderer::strokeRectangle(const Rectangle& bounds, const uint16_t cornerRadius, const Color* color) {
		if (cornerRadius > 0) {
			strokeHorizontalLine(
				Point(bounds.getX() + cornerRadius, bounds.getY()),
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);

			strokeHorizontalLine(
				Point(bounds.getX() + cornerRadius, bounds.getY() + bounds.getHeight() - 1),
				bounds.getWidth() - cornerRadius - cornerRadius,
				color
			);

			strokeVerticalLine(
				Point(bounds.getX() , bounds.getY() + cornerRadius),
				bounds.getHeight() - cornerRadius - cornerRadius,
				color
			);

			strokeVerticalLine(
				Point(bounds.getX() + bounds.getWidth() - 1, bounds.getY() + cornerRadius),
				bounds.getHeight() - cornerRadius - cornerRadius,
				color
			);

			strokeRoundedCorners(
				Point(bounds.getX() + cornerRadius, bounds.getY() + cornerRadius),
				cornerRadius,
				1,
				color
			);

			strokeRoundedCorners(
				Point(bounds.getX() + bounds.getWidth() - cornerRadius - 1, bounds.getY() + cornerRadius),
				cornerRadius,
				2,
				color
			);

			strokeRoundedCorners(
				Point(bounds.getX() + bounds.getWidth() - cornerRadius - 1, bounds.getY() + bounds.getHeight() - cornerRadius - 1),
				cornerRadius,
				4,
				color
			);

			strokeRoundedCorners(
				Point(bounds.getX() + cornerRadius, bounds.getY() + bounds.getHeight() - cornerRadius - 1),
				cornerRadius,
				8,
				color
			);
		}
		else {
			strokeRectangle(bounds, color);
		}
	}

	void Renderer::strokeLine(const Point& from, const Point& to, const Color* color) {
		// Vertical line
		if (from.getX() == to.getX()) {
			strokeVerticalLine(
				Point(from.getX(), std::min(from.getY(), to.getY())),
				std::abs(to.getY() - from.getY()) + 1,
				color
			);
		}
		// Horizontal line
		else if (from.getY() == to.getY()) {
			strokeHorizontalLine(
				Point(std::min(from.getX(), to.getX()), from.getY()),
				std::abs(to.getX() - from.getX()) + 1,
				color
			);
		}
		// Meh...
		else {
			const auto& clip = getClip();
			const auto clipX2 = clip.getX2();
			const auto clipY2 = clip.getY2();

			if (
				(from.getX() < clip.getX() && to.getX() < clip.getX())
				|| (from.getX() > clipX2 && to.getX() > clipX2)

				|| (from.getY() < clip.getY() && to.getY() < clip.getY())
				|| (from.getY() > clipY2 && to.getY() > clipY2)
			)
				return;

			int32_t
				x1 = from.getX(),
				y1 = from.getY(),

				x2 = to.getX(),
				y2 = to.getY();

			const bool deltaYGreater = std::abs(y2 - y1) > std::abs(x2 - x1);

			if (deltaYGreater) {
				std::swap(x1, y1);
				std::swap(x2, y2);
			}

			if (x1 > x2) {
				std::swap(x1, x2);
				std::swap(y1, y2);
			}

			const int32_t deltaX = x2 - x1;
			const int32_t deltaY = std::abs(y2 - y1);
			const int32_t yStep = y1 > y2 ? -1 : 1;
			int32_t partRemaining = deltaX >> 1;
			int32_t partVarFrom = x1;

			uint16_t partLength = 0;

			if (deltaYGreater) {
				while (x1 <= x2) {
					partLength++;
					partRemaining -= deltaY;

					if (partRemaining < 0) {
						partRemaining += deltaX;

						if (partLength == 1) {
							putPixel(Point(y1, partVarFrom), color);
						}
						else {
							strokeVerticalLine(Point(y1, partVarFrom), partLength, color);
						}

						partLength = 0;
						partVarFrom = x1 + 1;
						y1 += yStep;
					}

					x1++;
				}

				if (partLength > 0)
					strokeVerticalLine(Point(y1, partVarFrom), partLength, color);
			}
			else {
				while (x1 <= x2) {
					partLength++;
					partRemaining -= deltaY;

					if (partRemaining < 0) {
						partRemaining += deltaX;

						if (partLength == 1) {
							putPixel(Point(partVarFrom, y1), color);
						}
						else {
							strokeHorizontalLine(Point(partVarFrom, y1), partLength, color);
						}

						partLength = 0;
						partVarFrom = x1 + 1;
						y1 += yStep;
					}

					x1++;
				}

				if (partLength > 0)
					strokeHorizontalLine(Point(partVarFrom, y1), partLength, color);
			}
		}
	}

	void Renderer::strokeLine(const Point& from, const Point& to, const Color* color, const uint8_t thickness) {
		if (thickness > 1) {
			// Same Y, horizontal line
			if (from.getY() == to.getY()) {
				fillRectangle(
					Rectangle(
						std::min(from.getX(), to.getX()),
						from.getY() - thickness / 2,
						std::abs(to.getX() - from.getX()),
						thickness
					),
					color
				);

				return;
			}

			// Same X, vertical line
			if (from.getX() == to.getX()) {
				fillRectangle(
					Rectangle(
						from.getX() - thickness / 2,
						std::min(from.getY(), to.getY()),
						thickness,
						std::abs(to.getY() - from.getY())
					),
					color
				);

				return;
			}

			// Interpolated line
			auto p1 = from;
			auto p2 = to;

			const int32_t dx = p2.getX() - p1.getX();
			const int32_t dy = p2.getY() - p1.getY();

			// If angle < 45 deg, subdividing rendering by X
			if (abs(dx) >= abs(dy)) {
				if (p1.getX() > p2.getX())
					std::swap(p1, p2);

				for (int32_t x = p1.getX(); x <= p2.getX(); x++) {
					const int32_t y = p1.getY() + dy * (x - p1.getX()) / dx;
					strokeVerticalLine(Point(x, y - thickness / 2), thickness, color);
				}
			}
			// Else by Y
			else {
				if (p1.getY() > p2.getY())
					std::swap(p1, p2);

				for (int y = p1.getY(); y <= p2.getY(); y++) {
					const int32_t x = p1.getX() + dx * (y - p1.getY()) / dy;
					strokeHorizontalLine(Point(x - thickness / 2, y), thickness, color);
				}
			}
		}
		else {
			strokeLine(from, to, color);
		}
	}
	
	void Renderer::strokeTriangle(const Point& position1, const Point& position2, const Point& position3, const Color* color) {
		// Simple as fuck
		strokeLine(position1, position2, color);
		strokeLine(position2, position3, color);
		strokeLine(position3, position1, color);
	}

	void Renderer::fillTriangle(const Point& position1, const Point& position2, const Point& position3, const Color* color) {
		int32_t
			x1 = position1.getX(),
			y1 = position1.getY(),

			x2 = position2.getX(),
			y2 = position2.getY(),

			x3 = position3.getX(),
			y3 = position3.getY();

		// Sort coordinates by Y order (y2 >= y1 >= y0)
		if (y1 > y2) {
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		if (y2 > y3) {
			std::swap(y3, y2);
			std::swap(x3, x2);
		}

		if (y1 > y2) {
			std::swap(y1, y2);
			std::swap(x1, x2);
		}

		int32_t a, b;

		// No triangle here, just single line
		if (y1 == y3) {
			a = b = x1;

			if (x2 < a) {
				a = x2;
			}
			else if (x2 > b) {
				b = x2;
			}

			if (x3 < a) {
				a = x3;
			}
			else if (x3 > b) {
				b = x3;
			}

			strokeHorizontalLine(Point(a, y1), b - a + 1, color);

			return;
		}

		const int32_t dx12 = x2 - x1;
		const int32_t dy12 = y2 - y1;
		const int32_t dx13 = x3 - x1;
		const int32_t dy13 = y3 - y1;
		const int32_t dx23 = x3 - x2;
		const int32_t dy23 = y3 - y2;
		int32_t sa = 0;
		int32_t sb = 0;
		int32_t last;
		int32_t y;

		// For upper part of triangle, find scanline crossings for segments
		// 0-1 and 0-2.  If y1=y2 (flat-bottomed triangle), the scanline y1
		// is included here (and second loop will be skipped, avoiding a /0
		// error there), otherwise scanline y1 is skipped here and handled
		// in the second loop...which also avoids a /0 error here if y0=y1
		// (flat-topped triangle).
		if (y2 == y3) {
			last = y2;  // Include y1 scanline
		}
		else {
			last = y2 - 1; // Skip it
		}

		for (y = y1; y <= last; y++) {
			a = x1 + sa / dy12;
			b = x1 + sb / dy13;
			sa += dx12;
			sb += dx13;

			if (a > b)
				std::swap(a, b);

			strokeHorizontalLine(Point(a, y), b - a + 1, color);
		}

		// For lower part of triangle, find scanline crossings for segments
		// 0-2 and 1-2.  This loop is skipped if y1=y2.
		sa = dx23 * (y - y2);
		sb = dx13 * (y - y1);

		while (y <= y3) {
			a = x2 + sa / dy23;
			b = x1 + sb / dy13;
			sa += dx23;
			sb += dx13;

			if (a > b)
				std::swap(a, b);

			strokeHorizontalLine(Point(a, y), b - a + 1, color);

			y++;
		}
	}

	void Renderer::strokeCircle(const Point& center, uint16_t radius, const Color* color) {
		if (radius == 0 || !color)
			return;

		if (radius == 1) {
			putPixel(center, color);
			return;
		}

		int32_t
			f = 1 - radius,
			ddF_y = -2 * radius,
			ddF_x = 1,
			xs = -1,
			xe = 0,
			len;

		bool first = true;

		do {
			while (f < 0) {
				++xe;
				f += (ddF_x += 2);
			}

			f += (ddF_y += 2);

			if (xe-xs > 1) {
				if (first) {
					len = 2 * (xe - xs) - 1;
					strokeHorizontalLine(Point(center.getX() - xe, center.getY() + radius), len, color);
					strokeHorizontalLine(Point(center.getX() - xe, center.getY() - radius), len, color);

					strokeVerticalLine(Point(center.getX() + radius, center.getY() - xe), len, color);
					strokeVerticalLine(Point(center.getX() - radius, center.getY() - xe), len, color);

					first = false;
				}
				else {
					len = xe - xs++;
					strokeHorizontalLine(Point(center.getX() - xe, center.getY() + radius), len, color);
					strokeHorizontalLine(Point(center.getX() - xe, center.getY() - radius), len, color);
					strokeHorizontalLine(Point(center.getX() + xs, center.getY() - radius), len, color);
					strokeHorizontalLine(Point(center.getX() + xs, center.getY() + radius), len, color);

					strokeVerticalLine(Point(center.getX() + radius, center.getY() + xs), len, color);
					strokeVerticalLine(Point(center.getX() + radius, center.getY() - xe), len, color);
					strokeVerticalLine(Point(center.getX() - radius, center.getY() - xe), len, color);
					strokeVerticalLine(Point(center.getX() - radius, center.getY() + xs), len, color);
				}
			}
			else {
				++xs;
				putPixel(Point(center.getX() - xe, center.getY() + radius), color);
				putPixel(Point(center.getX() - xe, center.getY() - radius), color);
				putPixel(Point(center.getX() + xs, center.getY() - radius), color);
				putPixel(Point(center.getX() + xs, center.getY() + radius), color);

				putPixel(Point(center.getX() + radius, center.getY() + xs), color);
				putPixel(Point(center.getX() + radius, center.getY() - xe), color);
				putPixel(Point(center.getX() - radius, center.getY() - xe), color);
				putPixel(Point(center.getX() - radius, center.getY() + xs), color);
			}

			xs = xe;
		}
		while (xe < --radius);
	}

	void Renderer::strokeCircle(const Point& center, const uint16_t outerRadius, const uint16_t thickness, const Color* color) {
		if (thickness == 0) {
			return;
		}
		else if (thickness == 1) {
			strokeCircle(center, outerRadius, color);
			return;
		}

		const int32_t innerRadius = static_cast<int32_t>(outerRadius) - thickness;

		if (innerRadius <= 0) {
			fillCircle(center, outerRadius, color);
			return;
		}

		if (!color)
			return;

		// Квадраты радиусов для проверки условий (чтобы избежать sqrt)
		const int32_t outerRadiusSq = static_cast<int32_t>(outerRadius) * outerRadius;
		const int32_t innerRadiusSq = innerRadius * innerRadius;

		// Начальные X-координаты для верхних/нижних точек (y = 0)
		int16_t xOut = outerRadius;
		int16_t xIn = innerRadius;

		// Идем строго по строкам от центральной оси Y вверх и вниз
		for (int16_t y = 0; y <= outerRadius; ++y) {
			const int32_t ySq = static_cast<int32_t>(y) * y;

			// Корректируем внешнюю границу X для текущего Y
			while (xOut >= 0 && (static_cast<int32_t>(xOut) * xOut + ySq) > outerRadiusSq) {
				xOut--;
			}

			// Корректируем внутреннюю границу X для текущего Y
			while (xIn >= 0 && (static_cast<int32_t>(xIn) * xIn + ySq) > innerRadiusSq) {
				xIn--;
			}

			// Левая граница закраски — сразу за внутренним радиусом, правая — внешний радиус
			const int16_t xLeftBoundary = xIn + 1;
			const int16_t xRightBoundary = xOut;

			if (xLeftBoundary <= xRightBoundary) {
				const uint16_t length = xRightBoundary - xLeftBoundary + 1;

				// 1. Отрисовка строки в верхней половине (+y)
				// Правый сектор
				strokeHorizontalLine(Point{static_cast<int16_t>(center.getX() + xLeftBoundary), static_cast<int16_t>(center.getY() + y)}, length, color);
				// Левый сектор (зеркально)
				strokeHorizontalLine(Point{static_cast<int16_t>(center.getX() - xRightBoundary), static_cast<int16_t>(center.getY() + y)}, length, color);

				// 2. Отрисовка строки в нижней половине (-y) — пропускаем y=0, чтобы не дублировать центр
				if (y > 0) {
					// Правый сектор
					strokeHorizontalLine(Point{static_cast<int16_t>(center.getX() + xLeftBoundary), static_cast<int16_t>(center.getY() - y)}, length, color);
					// Левый сектор (зеркально)
					strokeHorizontalLine(Point{static_cast<int16_t>(center.getX() - xRightBoundary), static_cast<int16_t>(center.getY() - y)}, length, color);
				}
			}
		}
	}

	void Renderer::fillCircle(const Point& center, uint16_t radius, const Color* color) {
		if (radius == 0 || !color)
			return;

		if (radius == 1) {
			putPixel(center, color);
			return;
		}

		int32_t x = 0;
		int32_t dx = 1;
		int32_t dy = radius + radius;
		int32_t p = -(radius >> 1);

		strokeHorizontalLine(Point(center.getX() - radius, center.getY()), dy + 1, color);

		while(x < radius){
			if (p>= 0) {
				strokeHorizontalLine(Point(center.getX() - x, center.getY() + radius), dx, color);
				strokeHorizontalLine(Point(center.getX() - x, center.getY() - radius), dx, color);

				dy -= 2;
				p -= dy;
				radius--;
			}

			dx += 2;
			p += dx;
			x++;

			strokeHorizontalLine(Point(center.getX() - radius, center.getY() + x), dy + 1, color);
			strokeHorizontalLine(Point(center.getX() - radius, center.getY() - x), dy + 1, color);
		}
	}

	void Renderer::strokeArc(const Point& center, const uint16_t radius, float fromAngleRad,  float arcMeasureRad, const Color* color) {
		// Nothing to draw
		if (arcMeasureRad <= 0)
			return;

		if (arcMeasureRad > Math::twoPi)
			arcMeasureRad = Math::twoPi;

		// 360 deg circle
		if (arcMeasureRad == Math::twoPi) {
			strokeCircle(center, radius, color);
			return;
		}

		// Normalizing angles
		fromAngleRad = Math::normalizeAngleRad2Pi(fromAngleRad);
		const auto toAngleRad = Math::normalizeAngleRad2Pi(fromAngleRad + arcMeasureRad);
		const auto arcMeasureGreaterPi = arcMeasureRad > Math::pi;

		// Computing trigonometric values only once
		const auto cosFrom = std::cosf(fromAngleRad);
		const auto sinFrom = std::sinf(fromAngleRad);
		const auto cosTo = std::cosf(toAngleRad);
		const auto sinTo = std::sinf(toAngleRad);

		// Bresenham's algorithm
		int16_t x = 0;
		int16_t y = radius;
		int16_t d = 3 - 2 * radius;

		const auto isPointInArc = [&](const int16_t dx, const int16_t dy) noexcept {
			const auto dyF = static_cast<float>(dy);
			const auto dxF = -static_cast<float>(dx);

			// Cross product with the initial angle vectors (CCW)
			const auto gteFrom = dxF * sinFrom - dyF * cosFrom >= 0.0f;
			const auto lteTo = dxF * sinTo - dyF * cosTo <= 0.0f;

			// If sweep angle > 180 deg
			return
				arcMeasureGreaterPi
				? gteFrom || lteTo
				: gteFrom && lteTo;
		};

		auto checkAndPutPixel = [&](const int16_t dx, const int16_t dy) noexcept {
			if (isPointInArc(dx, dy)) {
				putPixel({ center.getX() + dx, center.getY() + dy }, color);
			}
		};

		// 8 octants at once
		while (x <= y) {
			checkAndPutPixel(y, -x);
			checkAndPutPixel(y, x);

			checkAndPutPixel(x, -y);
			checkAndPutPixel(-x, -y);

			checkAndPutPixel(-y, -x);
			checkAndPutPixel(-y, x);

			checkAndPutPixel(x, y);
			checkAndPutPixel(-x, y);

			if (d >= 0) {
				d += 4 * (x - y) + 10;
				y--;
			}
			else {
				d += 4 * x + 6;
			}

			x++;
		}
	}

	void Renderer::strokeArc(const Point& center, const uint16_t outerRadius, const uint16_t thickness, float fromAngleRad, float arcMeasureRad, const Color* color) {
		// Nothing to draw
		if (thickness == 0) {
		    return;
	    }
		// Simple non-THICC arc
		else if (thickness == 1) {
			strokeArc(center, outerRadius, fromAngleRad, arcMeasureRad, color);
			return;
		}

		if (arcMeasureRad <= 0)
			return;

		if (arcMeasureRad > Math::twoPi)
			arcMeasureRad = Math::twoPi;

		if (arcMeasureRad == Math::twoPi) {
			strokeCircle(center, outerRadius, color);
			return;
		}

		fromAngleRad = Math::normalizeAngleRad2Pi(fromAngleRad);
		const auto toAngleRad = Math::normalizeAngleRad2Pi(fromAngleRad + arcMeasureRad);
		const auto arcMeasureGreaterPi = arcMeasureRad > Math::pi;

	    const int32_t innerRadius = static_cast<int32_t>(outerRadius) - thickness;
	    const int32_t outerRadiusSq = static_cast<int32_t>(outerRadius) * outerRadius;
	    const int32_t innerRadiusSq = innerRadius * innerRadius;

	    const float cosFrom = std::cos(fromAngleRad);
	    const float sinFrom = std::sin(fromAngleRad);
	    const float cosTo = std::cos(toAngleRad);
	    const float sinTo = std::sin(toAngleRad);

	    int16_t xOut = outerRadius;
	    int16_t xIn = innerRadius;

		const auto isPointInArc = [&](const int16_t dx, const int16_t dy) noexcept {
			const auto dyF = static_cast<float>(dy);
			const auto dxF = -static_cast<float>(dx);

			const auto gteFrom = dxF * sinFrom - dyF * cosFrom >= 0.0f;
			const auto lteTo = dxF * sinTo - dyF * cosTo <= 0.0f;

			return
				arcMeasureGreaterPi
				? gteFrom || lteTo
				: gteFrom && lteTo;
		};

	    for (int16_t y = 0; y <= outerRadius; ++y) {
	        const int32_t ySq = static_cast<int32_t>(y) * y;

	        while (xOut >= 0 && (static_cast<int32_t>(xOut) * xOut + ySq) > outerRadiusSq) {
	            xOut--;
	        }
	        while (xIn >= 0 && (static_cast<int32_t>(xIn) * xIn + ySq) > innerRadiusSq) {
	            xIn--;
	        }

	        int16_t xLeftBoundary = xIn + 1;
	        int16_t xRightBoundary = xOut;

	        if (xLeftBoundary <= xRightBoundary) {
	            const auto drawSegmentInRow = [&](const int16_t ySign) noexcept {
	                const int16_t currentY = center.getY() + y * ySign;
	                int16_t startX = -1;
	                uint16_t length = 0;

	               const auto flushSegment = [&] noexcept {
	                    if (length > 0) {
	                        strokeHorizontalLine({ startX, currentY }, length, color);
	                        length = 0;
	                        startX = -1;
	                    }
	                };

	            	// Right circle side (center + x)
	                for (int16_t x = xLeftBoundary; x <= xRightBoundary; ++x) {
	                    if (isPointInArc(x, y * ySign)) {
	                        if (startX == -1)
	                        	startX = center.getX() + x;

	                        length++;
	                    }
	                	else {
	                        flushSegment();
	                    }
	                }
	                flushSegment();

	            	// Left circle side (center - x)
	                for (int16_t x = xRightBoundary; x >= xLeftBoundary; --x) {
	                    if (isPointInArc(-x, y * ySign)) {
	                        if (startX == -1)
	                        	startX = center.getX() - x;

	                        length++;
	                    }
	                	else {
	                        flushSegment();
	                    }
	                }
	                flushSegment();
	            };

	            // Upper circle row (y + 1)
	            drawSegmentInRow(1);

	        	// Lower circle row (y - 1) EXCEPT y = 0 (already drawn in upper)
	            if (y > 0) {
	                drawSegmentInRow(-1);
	            }
	        }
	    }
	}
	
	void Renderer::strokeCatmullRomSpline(const Point* points, const size_t pointsLength, const Color* color, const uint16_t segmentsPerCurve, const float tension) {
		assert(pointsLength >= 4 && 1);

		Point pointPrev {};
		bool pizda = false;

		for (size_t pointIndex = 0; pointIndex < pointsLength - 3; pointIndex++) {
			for (uint16_t segment = 0; segment <= segmentsPerCurve; segment++) {
				const float t = static_cast<float>(segment) / static_cast<float>(segmentsPerCurve);
				const auto point = getCatmullRomPoint(points, pointIndex, t, tension);

				// Linking to prev point
				if (pizda)
					strokeLine(pointPrev, point, color);

				pointPrev = point;
				pizda = true;
			}
		}
	}

	Point Renderer::getCatmullRomPoint(const Point* points, const size_t index, const float t, const float tension) {
		// CatmullRom matrix with tension support
		const float m00 = -tension;
		const float m01 = 2 - tension;
		const float m02 = tension - 2;
		const float m03 = tension;
		const float m10 = 2 * tension;
		const float m11 = tension - 3;
		const float m12 = 3 - 2 * tension;
		const float m13 = -tension;
		const float m20 = -tension;
		constexpr float m21 = 0;
		const float m22 = tension;
		constexpr float m23 = 0;
		constexpr float m30 = 0;
		constexpr float m31 = 1;
		constexpr float m32 = 0;
		constexpr float m33 = 0;

		// Spline coefficients
		const float t2 = t * t;
		const float t3 = t2 * t;

		// Vector T: [t³, t², t, 1]
		const float tx = m00 * t3 + m10 * t2 + m20 * t + m30;
		const float ty = m01 * t3 + m11 * t2 + m21 * t + m31;
		const float tz = m02 * t3 + m12 * t2 + m22 * t + m32;
		const float tw = m03 * t3 + m13 * t2 + m23 * t + m33;

		// Multiplying T on control point matrix
		return {
			static_cast<int32_t>(tx * points[index].getX() + ty * points[index + 1].getX() + tz * points[index + 2].getX() + tw * points[index + 3].getX()),
			static_cast<int32_t>(tx * points[index].getY() + ty * points[index + 1].getY() + tz * points[index + 2].getY() + tw * points[index + 3].getY())
		};
	}

	float Renderer::getAtan2Fast(const float y, const float x) {
		// http://pubs.opengroup.org/onlinepubs/009695399/functions/atan2.html
		// Volkan SALMA

		constexpr float ONEQTR_PI = Math::pi / 4.0;
		constexpr float THRQTR_PI = 3.0 * Math::pi / 4.0;

		float r, angle;
		const float abs_y = std::fabs(y) + 1e-10f;      // kludge to prevent 0/0 condition

		if (x < 0.0f) {
			r = (x + abs_y) / (abs_y - x);
			angle = THRQTR_PI;
		}
		else {
			r = (x - abs_y) / (x + abs_y);
			angle = ONEQTR_PI;
		}

		angle += (0.1963f * r * r - 0.9817f) * r;

		if (y < 0.0f)
			return -angle;     // negate if in quad III or IV

		return angle;
	}

	void Renderer::strokeRoundedCorners(const Point& center, int32_t radius, const uint8_t corner, const Color* color) {
		int32_t f     = 1 - radius;
		int32_t ddF_x = 1;
		int32_t ddF_y = -2 * radius;
		int32_t xe    = 0;
		int32_t xs    = 0;
		int32_t len   = 0;

		do
		{
			while (f < 0) {
				++xe;
				f += (ddF_x += 2);
			}
			f += (ddF_y += 2);

			if (xe-xs==1) {
				if (corner & 0x1) { // left top
					putPixel(Point(center.getX() - xe, center.getY() - radius), color);
					putPixel(Point(center.getX() - radius, center.getY() - xe), color);
				}

				if (corner & 0x2) { // right top
					putPixel(Point(center.getX() + radius    , center.getY() - xe), color);
					putPixel(Point(center.getX() + xs + 1, center.getY() - radius), color);
				}

				if (corner & 0x4) { // right bottom
					putPixel(Point(center.getX() + xs + 1, center.getY() + radius), color);
					putPixel(Point(center.getX() + radius, center.getY() + xs + 1), color);
				}

				if (corner & 0x8) { // left bottom
					putPixel(Point(center.getX() - radius, center.getY() + xs + 1), color);
					putPixel(Point(center.getX() - xe, center.getY() + radius) , color);
				}
			}
			else {
				len = xe - xs++;
				
				if (corner & 0x1) { // left top
					strokeHorizontalLine(Point(center.getX() - xe, center.getY() - radius), len, color);
					strokeVerticalLine(Point(center.getX() - radius, center.getY() - xe), len, color);
				}

				if (corner & 0x2) { // right top
					strokeVerticalLine(Point(center.getX() + radius, center.getY() - xe), len, color);
					strokeHorizontalLine(Point(center.getX() + xs, center.getY() - radius), len, color);
				}

				if (corner & 0x4) { // right bottom
					strokeHorizontalLine(Point(center.getX() + xs, center.getY() + radius), len, color);
					strokeVerticalLine(Point(center.getX() + radius, center.getY() + xs), len, color);
				}

				if (corner & 0x8) { // left bottom
					strokeVerticalLine(Point(center.getX() - radius, center.getY() + xs), len, color);
					strokeHorizontalLine(Point(center.getX() - xe, center.getY() + radius), len, color);
				}
			}
			xs = xe;
		} while (xe < radius--);
	}

	void Renderer::fillUpperRoundedCorners(const Point& center, uint16_t radius, const int32_t delta, const Color* color) {
		int32_t f = 1 - radius;
		int32_t ddF_x = 1;
		int32_t ddF_y = -radius - radius;
		int32_t y = 0;
		int32_t lineLength;

		while (y < radius) {
			if (f >= 0) {
				lineLength = y + y + delta;

				if (lineLength > 0) {
					strokeHorizontalLine(Point(center.getX() - y, center.getY() - radius), lineLength, color);
				}

				radius--;
				ddF_y += 2;
				f += ddF_y;
			}

			y++;
			ddF_x += 2;
			f += ddF_x;
			lineLength = radius + radius + delta;

			if (lineLength > 0) {
				strokeHorizontalLine(Point(center.getX() - radius, center.getY() - y), lineLength, color);
			}
		}
	}

	void Renderer::fillLowerRoundedCorners(const Point& center, uint16_t radius, const int32_t delta, const Color* color) {
		int32_t f = 1 - radius;
		int32_t ddF_x = 1;
		int32_t ddF_y = -radius - radius;
		int32_t y = 0;
		int32_t lineLength;

		while (y < radius) {
			if (f >= 0) {
				lineLength = y + y + delta;

				if (lineLength > 0) {
					strokeHorizontalLine(Point(center.getX() - y, center.getY() + radius), lineLength, color);
				}

				radius--;
				ddF_y += 2;
				f += ddF_y;
			}

			y++;
			ddF_x += 2;
			f += ddF_x;
			lineLength = radius + radius + delta;

			if (lineLength > 0) {
				strokeHorizontalLine(Point(center.getX() - radius, center.getY() + y), lineLength, color);
			}
		}
	}

	void Renderer::putMissingGlyph(const Point& position, const Font* font, const uint8_t fontScale, const Color* color) {
		strokeRectangle(
			Rectangle(
				position.getX(),
				position.getY(),
				Font::missingGlyphWidth * fontScale,
				font->getLineHeight() * fontScale
			),
			color
		);
	}

	void Renderer::putGlyph(const Point& position, const Font* font, const uint8_t fontScale, const Color* color, const int32_t glyphIndex, const Glyph* glyph) {
		auto bitIndex = font->getBitmapBitIndex(glyphIndex, glyph);

		auto bitmapPtr = font->getBitmap() + (bitIndex / 8);
		bitIndex = bitIndex % 8;

		const int32_t x1 = position.getX();
		const int32_t y1 = position.getY();

		// Non-scaled fonts can be drawn a little bit faster
		if (fontScale == 1) {
			const int32_t x2 = x1 + font->getWidth(glyph);
			const int32_t y2 = y1 + font->getLineHeight();

			for (int32_t y = y1; y < y2; y++) {
				for (int32_t x = x1; x < x2; x++) {
					if ((*bitmapPtr >> bitIndex) & 1)
						putPixel(Point(x, y), color);

					bitIndex++;

					if (bitIndex > 7) {
						bitIndex = 0;
						bitmapPtr++;
					}
				}
			}
		}
		// SLOWMO
		else {
			const int32_t x2 = x1 + font->getWidth(glyph) * fontScale;
			const int32_t y2 = y1 + font->getLineHeight() * fontScale;

			for (int32_t y = y1; y < y2; y += fontScale) {
				for (int32_t x = x1; x < x2; x += fontScale) {
					if ((*bitmapPtr >> bitIndex) & 1)
						fillRectangle(Rectangle(x, y, fontScale, fontScale), color);

					bitIndex++;

					if (bitIndex > 7) {
						bitIndex = 0;
						bitmapPtr++;
					}
				}
			}
		}
	}

	void Renderer::putText(const Point& position, const Font* font, const Color* color, const uint32_t codepoint) {
		putText(position, font, 1, color, codepoint);
	}

	void Renderer::putText(const Point& position, const Font* font, const uint8_t fontScale, const Color* color, const uint32_t codepoint) {
		const auto& clip = getClip();
		const auto clipX2 = clip.getX2();

		if (
			position.getX() > clipX2
			|| position.getY() > clip.getY2()
			|| position.getY() + font->getLineHeight(fontScale) < clip.getY()
			|| !color
		)
			return;

		const auto glyphIndex = font->getGlyphIndex(codepoint);

		if (glyphIndex >= 0) {
			const auto glyph = font->getGlyphByIndex(glyphIndex);

			if (position.getX() + font->getWidth(fontScale, glyph) < clip.getX())
				return;

			putGlyph(
				position,
				font,
				fontScale,
				color,
				glyphIndex,
				glyph
			);
		}
		else {
			putMissingGlyph(position, font, fontScale, color);
		}
	}

	void Renderer::putText(const Point& position, const Font* font, const uint8_t fontScale, const Color* color, const std::string_view text) {
		const auto& clip = getClip();
		const auto clipX2 = clip.getX2();

		// Skipping rendering if text is obviously not in clip region
		if (
			position.getX() > clipX2
			|| position.getY() > clip.getY2()
			|| position.getY() + font->getLineHeight(fontScale) < clip.getY()
			|| !color
		)
			return;

		size_t charIndex = 0;

		auto x = position.getX();
		const auto y = position.getY();

		uint32_t codepoint;
		int32_t glyphIndex;
		const Glyph* glyph;
		int32_t glyphX2;
		uint8_t glyphWidth;

		while (charIndex < text.length()) {
			// Trying to find glyph matched to char
			UTF8::nextCodepoint(text, charIndex, codepoint);
			glyphIndex = font->getGlyphIndex(codepoint);

			// If glyph was found
			if (glyphIndex >= 0) {
				glyph = font->getGlyphByIndex(glyphIndex);
				glyphWidth = font->getWidth(fontScale, glyph);

				// If glyph can be rendered as "human-readable"
				// For example, U+007F "DE" symbol often has zero width in some fonts
				if (glyphWidth > 0) {
					glyphX2 = x + glyphWidth;

					// Rendering current glyph only if it's in clip region
					if (glyphX2 > clip.getX()) {
						putGlyph(
							Point(x, y),
							font,
							fontScale,
							color,
							glyphIndex,
							glyph
						);
					}

					x = glyphX2;
				}
				else {
					putMissingGlyph(Point(x, y), font, fontScale, color);

					x += Font::missingGlyphWidth * fontScale;
				}
			}
			else {
				putMissingGlyph(Point(x, y), font, fontScale, color);

				x += Font::missingGlyphWidth * fontScale;
			}

			// Stopping rendering if next glyphs will not be in clip region
			if (x > clipX2)
				break;
		}
	}

	void Renderer::putText(const Point& position, const Font* font, const Color* color, const std::string_view text) {
		putText(position, font, 1, color, text);
	}

	void Renderer::flush() {

	}
}