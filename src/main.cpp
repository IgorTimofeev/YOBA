#include <Arduino.h>

#include "hardware/screen/drivers/ILI9341Driver.h"
#include "hardware/screen/buffers/bit8PaletteBuffer.h"
#include "hardware/input/touch/FT6336UTouchPanel.h"
#include "color.h"
#include "ui/shapes/rectangle.h"
#include "ui/text.h"
#include "ui/application.h"
#include "ui/debug/touchCanvas.h"
#include "ui/debug/testView.h"
#include "resources/fonts/PIXY10Font.h"

using namespace yoba;

ILI9341Driver screenDriver = ILI9341Driver(
	ScreenOrientation::Landscape270,
	5,
	16,
	17
);

Bit8PaletteBuffer screenBuffer = Bit8PaletteBuffer(&screenDriver);

FT6336UTouchPanel touchPanel = FT6336UTouchPanel(26, 32);

Application application = Application(&screenBuffer);

Bit8PaletteColor textColor = Bit8PaletteColor(255);

TestView paletteView;
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
	screenBuffer.setOpenComputersPaletteColors();
	screenBuffer.setPaletteColor(0, Rgb888Color(0, 0, 0));

	// Starting application
	touchPanel.setup();
	application.addInputDevice(&touchPanel);

	application.setup();

	// Adding UI elements
	application += &paletteView;

	text.setAlignment(Alignment::Center);
	application += &text;

	touchCanvas.setForegroundColor(&textColor);
	application += &touchCanvas;
}

void loop() {
	auto startTime = millis();

	// Text
	wchar_t penis[255];
	swprintf(penis, 255, L"Аптайм ванючьий: %f s", (float) millis() / 1000.0f);
	text.setText(penis);

	application.tick();

	auto deltaTime = max(millis() - startTime, 1UL);

	Serial.printf("FPS: %lu, free heap: %d kb, max alloc heap: %d kb\n", 60000 / deltaTime, ESP.getFreeHeap() / 1024, ESP.getMaxAllocHeap() / 1024);

	// svit slip....
	uint8_t desiredFPS = 60;
	uint32_t desiredDeltaTime = 1000 / desiredFPS;
	if (deltaTime < desiredDeltaTime)
		delay(desiredDeltaTime - deltaTime);

//	delay(1000);
}