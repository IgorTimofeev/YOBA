#include <Arduino.h>

#include "hardware/screen/buffers/screenBuffer.h"
#include "hardware/screen/drivers/ILI9341Driver.h"
#include "hardware/screen/buffers/eightBitsPaletteBuffer.h"
#include "hardware/touch/drivers/FT6336UDriver.h"
#include "color.h"
#include "fonts/Unscii16Font.h"
#include "fonts/Unscii8ThinFont.h"
#include "ui/shapes/rectangle.h"
#include "ui/text.h"
#include "ui/application.h"
#include "ui/debug/touchCanvas.h"
#include "ui/debug/paletteView.h"

using namespace yoba;

ILI9341Driver screenDriver = ILI9341Driver(
	5,
	16,
	17,
	ScreenOrientation::Landscape90
);

EightBitsPaletteBuffer screenBuffer = EightBitsPaletteBuffer(&screenDriver);

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

PaletteView paletteView = PaletteView();

ColorPalette textColor = ColorPalette(18);
Text text = Text(&textColor);

TouchCanvas touchCanvas;

void setup() {
	Serial.begin(115200);

	// Transceiver
	Serial.println("Disabling transceiver");
	pinMode(4, OUTPUT);
	digitalWrite(4, HIGH);

	// Display
	Serial.println("Updating palette");

	// ОПЕНКОМПЫ ЕПТЫ БЛЭЭ
	screenBuffer.fillWithOpenComputersColors();

	// Starting application
	application.begin();

	// Default values
	application.setDefaultFont(&font);

	// Adding UI elements
	application.addChild(&paletteView);

	text.setAlignment(Alignment::Center);
	application.addChild(&text);

	touchCanvas.setForeground(&textColor);
	application.addChild(&touchCanvas);
}

void loop() {
	auto startTime = millis();

	// Text
	text.setText(String("Uptime: ") + String((float) millis() / 1000.0f) + String(" s"));

	application.tick();

	auto deltaTime = max(millis() - startTime, 1UL);
//	auto fps = 60000 / deltaTime;
//	Serial.printf("FPS: %lu, free heap: %d kb, max alloc heap: %d kb\n", fps, ESP.getFreeHeap() / 1024, ESP.getMaxAllocHeap() / 1024);

	// svit slip....
	uint8_t desiredFPS = 120;
	uint32_t desiredDeltaTime = 1000 / desiredFPS;
	if (deltaTime < desiredDeltaTime)
		delay(desiredDeltaTime - deltaTime);

//	delay(1000);
}