#include <Arduino.h>
#include "hardware/screen/buffers/screenBuffer.h"
#include "hardware/screen/drivers/ILI9341Driver.h"
#include "hardware/screen/buffers/bits8PaletteBuffer.h"
#include "color.h"
#include "fonts/Unscii16Font.h"
#include "fonts/Unscii8ThinFont.h"
#include "number.h"

using namespace yoba;

ILI9341Driver driver = ILI9341Driver(
	5,
	16,
	17
);

EightBitsPaletteBuffer buffer = EightBitsPaletteBuffer(
	&driver,
	Size(320, 240)
);

Unscii16Font font = Unscii16Font();
//Unscii8ThinFont font = Unscii8ThinFont();

void setup() {
	Serial.begin(115200);

	// Transceiver
	Serial.println("Disabling transceiver");
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);

	// Display
	Serial.println("Updating palette");

	// Grayscale
	uint8_t govno = 0;

	for (int i = 0; i < 16; i++) {
		buffer.setPaletteColor(i, Color24(govno, govno, govno).to16Bit());
		govno += 0x11;
	}

	// RGB
	buffer.setPaletteColor(16, Color24(0xFF, 0x00, 0x00).to16Bit());
	buffer.setPaletteColor(17, Color24(0x00, 0xFF, 0x00).to16Bit());
	buffer.setPaletteColor(18, Color24(0x00, 0x00, 0xFF).to16Bit());

	Serial.println("Beginning display");
	buffer.begin();
//	screenBuffer->getDriver()->setTransactionBufferHeight(screenBuffer->getResolution().getHeight() / 2);
}

int32_t paletteIndex = 0;
int32_t paletteIndexInc = 1;
Point pivot = Point(10, 20);
int32_t pivotInc = 1;

void render() {
	// Clearing display
	buffer.clear(paletteIndex);

	paletteIndex += paletteIndexInc;

	if (paletteIndex > 15) {
		paletteIndex = 15;
		paletteIndexInc = -1;
	}
	else if (paletteIndex < 0) {
		paletteIndex = 0;
		paletteIndexInc = 1;
	}

	// Viewport
	uint16_t viewportPadding = 10;

	buffer.setViewport(Bounds(
		viewportPadding,
		viewportPadding,
		buffer.getSize().getWidth() - viewportPadding * 2,
		buffer.getSize().getHeight() - viewportPadding * 2
	));

	// Viewport visualization
	buffer.renderFilledRectangle(
		buffer.getViewport(),
		clamp(paletteIndex + paletteIndexInc, 0, 15)
	);

	// Dots
	for (int32_t x = pivot.getX(); x < pivot.getX() + 100; x += 5)
		buffer.renderPixel(Point(x, pivot.getY()), 16);

	// Horizontal line
	buffer.renderHorizontalLine(Point(pivot.getX(), pivot.getY() + 10), 100, 17);

	// Vertical line
	buffer.renderVerticalLine(Point(pivot.getX() + 110, pivot.getY() + 10), 40, 17);

	// Rect
	buffer.renderFilledRectangle(Bounds(pivot.getX(), pivot.getY() + 20, 100, 30), 18);

	// Text
	char pizda[255];
	sprintf(pizda, "Uptime: %.2f s", (float) millis() / 1000.0f);
	buffer.renderText(Point(pivot.getX(), pivot.getY() + 60), &font, 0, pizda);

	pivot.setX(pivot.getX() + pivotInc);
//	pivot.setY(pivot.getY() + pivotInc);

	if (pivot.getX() > buffer.getSize().getWidth())
		pivot.setX(0);

	if (pivot.getY() > buffer.getSize().getHeight())
		pivot.setY(0);

	// Line
	buffer.renderLine(Point(pivot.getX() + 5, pivot.getY() + 5), Point(pivot.getX() + 30, pivot.getY() + 130), 17);

	// Triangle
	buffer.renderTriangle(Point(pivot.getX(), pivot.getY() + 80), Point(pivot.getX() + 40, pivot.getY() + 85), Point(pivot.getX() + 22, pivot.getY() + 120), 18);

	// Filled triangle
	buffer.renderFilledTriangle(Point(pivot.getX() + 50, pivot.getY() + 80), Point(pivot.getX() + 90, pivot.getY() + 85), Point(pivot.getX() + 72, pivot.getY() + 120), 16);
}

void loop() {
	auto startTime = esp_timer_get_time();

	render();

	buffer.flush();

	auto deltaTime = esp_timer_get_time() - startTime;
	Serial.printf("FPS: %lld, color: %d, free heap: %d kb, max alloc heap: %d kb\n", 60000000 / deltaTime, paletteIndex, ESP.getFreeHeap() / 1024, ESP.getMaxAllocHeap() / 1024);

	// svit slip....
	uint32_t desiredDeltaTime = 1000 / 60;
	if (deltaTime < desiredDeltaTime)
		delay(desiredDeltaTime - deltaTime);

//	delay(1000);
}