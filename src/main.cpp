#include <Arduino.h>

#include "hardware/screen/buffers/screenBuffer.h"
#include "hardware/screen/drivers/ILI9341Driver.h"
#include "hardware/screen/buffers/eightBitsPaletteBuffer.h"
#include "hardware/touch/drivers/FT6336UDriver.h"
#include "color.h"
#include "fonts/Unscii16Font.h"
#include "fonts/Unscii8ThinFont.h"
#include "ui/rectangle.h"
#include "ui/text.h"
#include "ui/debug/TouchCanvas.h"
#include "ui/application.h"

using namespace yoba;

ILI9341Driver screenDriver = ILI9341Driver(
	5,
	16,
	17
);

EightBitsPaletteBuffer screenBuffer = EightBitsPaletteBuffer(
	&screenDriver,
	Size(320, 240),
	ScreenOrientation::Landscape0
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

ColorPalette backgroundColor = ColorPalette(0);
Rectangle backgroundRect = Rectangle(&backgroundColor);

ColorPalette textColor = ColorPalette(18);
Text text = Text(&font, &textColor);

int32_t backgroundPaletteIndex = 0;

TouchCanvas touchCanvas;

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

	// Starting application
	application.begin();

	// Adding UI elements
	application.addChild(&backgroundRect);

	text.setAlignment(Alignment::Center);
	application.addChild(&text);

	touchCanvas.setFont(&font);
	touchCanvas.setForeground(&textColor);
	application.addChild(&touchCanvas);

	application.addEventHandler([](Event& event) {
		if (event.getType() != EventType::TouchDown)
			return;

		backgroundPaletteIndex += 1;

		if (backgroundPaletteIndex >= 15) {
			backgroundPaletteIndex = 0;
		}

		backgroundColor.setIndex(backgroundPaletteIndex);
		backgroundRect.setFillColor(&backgroundColor);
	});
}

void loop() {
	auto startTime = millis();

	// Text
	text.setText(String("Index: ") + String(backgroundPaletteIndex) + String(", uptime: ") + String((float) millis() / 1000.0f) + String(" s"));

	application.tick();

	auto deltaTime = max(millis() - startTime, 1UL);
	auto fps = 60000 / deltaTime;
//	Serial.printf("FPS: %lu, free heap: %d kb, max alloc heap: %d kb\n", fps, ESP.getFreeHeap() / 1024, ESP.getMaxAllocHeap() / 1024);

	// svit slip....
	uint8_t desiredFPS = 60;
	uint32_t desiredDeltaTime = 1000 / desiredFPS;
	if (deltaTime < desiredDeltaTime)
		delay(desiredDeltaTime - deltaTime);

//	delay(1000);
}