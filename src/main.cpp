#include <Arduino.h>

#include "hardware/screen/buffers/screenBuffer.h"
#include "hardware/screen/drivers/ILI9341Driver.h"
#include "hardware/screen/buffers/eightBitsPaletteBuffer.h"
#include "hardware/touch/drivers/FT6336UDriver.h"
#include "color.h"
#include "fonts/Unscii16Font.h"
#include "fonts/Unscii8ThinFont.h"
#include "application.h"
#include "ui/rectangle.h"

using namespace yoba;

ILI9341Driver screenDriver = ILI9341Driver(
	5,
	16,
	17
);

EightBitsPaletteBuffer screenBuffer = EightBitsPaletteBuffer(
	&screenDriver,
	Size(320, 240)
);

FT6336UDriver touchDriver = FT6336UDriver(
	SDA,
	SCL,
	32,
	26
);

Application application = Application(
	&screenBuffer,
	&touchDriver
);

Unscii16Font font = Unscii16Font();

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
		screenBuffer.setPaletteColor(i, Color24(govno, govno, govno).toUint16());
		govno += 0x11;
	}

	// RGB
	screenBuffer.setPaletteColor(16, Color24(0xFF, 0x00, 0x00).toUint16());
	screenBuffer.setPaletteColor(17, Color24(0x00, 0xFF, 0x00).toUint16());
	screenBuffer.setPaletteColor(18, Color24(0x00, 0x00, 0xFF).toUint16());

	application.begin();

	application.getWorkspace().addChild(new Rectangle(16))
}

void loop() {
	auto startTime = esp_timer_get_time();

	application.tick();

	auto deltaTime = esp_timer_get_time() - startTime;
	Serial.printf("FPS: %lld, free heap: %d kb, max alloc heap: %d kb\n", 60000000 / deltaTime, ESP.getFreeHeap() / 1024, ESP.getMaxAllocHeap() / 1024);

	// svit slip....
	uint32_t desiredDeltaTime = 1000 / 60;
	if (deltaTime < desiredDeltaTime)
		delay(desiredDeltaTime - deltaTime);

//	delay(1000);
}