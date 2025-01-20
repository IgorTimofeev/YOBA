# Work in fucking progress

Inf days left, go away for now, touch some grass instead of me...

# YOBA | Your Breathtaking Application

A comprehensive UI framework written on modern C++ for embedded devices.

# Arduino example

```cpp
#include "yoba/src/main.h"
#include "yoba/src/ui.h"
#include "yoba/src/resources/fonts/UNSCII16Font.h"
#include "yoba/src/hardware/displays/ILI9341Display.h"
#include "yoba/src/hardware/touchPanels/FT6336UTouchPanel.h"

// Choosing hardware
ILI9341Display display = ILI9341Display(
    1, // Chip select pin
    2, // Data command pin
    3, // Reset pin
    40000000 // SPI frequency
);

FT6336UTouchPanel touchPanel = FT6336UTouchPanel(
    4, // SDA pin
    5, // SCL pin
    6 // Reset pin
    7 // Interrupt pin
);

// Using palette renderer, which is a good compromise between 
// appearance and performance. It stores colors as 8-bit indices 
// instead of raw RGB values and allows to develop wonderful 
// applications on microcontrollers with small amount of RAM
EightBitPaletteRenderer renderer = EightBitPaletteRenderer(
    3 // Count of colors in palette, up to 256
);

PaletteColor black(0);
PaletteColor white(1);
PaletteColor red(3);

// Choosing one of predefined sexy fonts for text rendering
UNSCII16Font font();

// Creating UI elements to interact with
Application application();
StackLayout stackLayout();
Text text();
Slider button();

// )))
uint16_t clickCount = 0;

void setup() {
    // Turning display on and attaching it to screen buffer
    // Then setting up touch panel and adding it as input device
    application.setup(&display, &renderer, &touchPanel);
    
    // Filling palette with colors in human-readable RGB format
    renderer.setPaletteColors({
        0x000000, // Black
        0xFFFFFF, // White
        0x990000 // Red
    })
    
    // Configuring UI
	
    // Text
    text.setHorizontalAlignment(Alignment::center);
    text.setFont(&font);
    text.setFontScale(2);
    text.setColor(&white);
    text.setText(L"");
    updateText();
	
    // Button
    button.setHeight(30);
    button.setCornerRadius(3);
    button.setFont(&font);
    button.setDefaultBackgroundColor(&black);
    button.setDefaultTextColor(&white);
    button.setPressedBackgroundColor(&white);
    button.setPressedTextColor(&black);
    button.setText(L"Click on me");
	
    button.isPressedChanged += [this]() {
        if (!button.isPressed())
            return;
        
        static std::wstringstream stream;
        stream.str(std::wstring());
        
        stream << L"Click count: " << clickCount++;
        
        button.setText(stream.str());
    };
    
    // Stack layout
    stackLayout.setSpacing(15);
    stackLayout.setMargin(Margin(40, 20));
    stackLayout += &text;
    stackLayout += &button;
    
    // Application
    application.setBackgroundColor(&red);
    application += &stackLayout;
}

void loop() {
    appplication.tick();
}
```

# ESP-IDF example

Same as Arduino, but

```cpp
// Everything before setup()

void main() {
    // Everything from setup()
    
    while (true) {
        // Everything from loop()
    }
}
```

# Platformio

C++17 or greater and RTTI are required, so you'll need to tweak `platformio.ini` like

```ini
[env:yobaExample]
platform = espressif32
board = esp32dev
framework = arduino

; This ↓
build_unflags =
	-fno-rtti
	-std=gnu++11
build_flags =
	-std=gnu++17
```
