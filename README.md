
# YOBA | Your Breathtaking Application

---

A comprehensive UI framework written on modern C++ for embedded devices.

# Arduino

```cpp
#include "yoba/src/main.h"
#include "yoba/src/ui.h"
#include "yoba/src/resources/fonts/UNSCII16Font.h"
#include "yoba/src/hardware/displays/ILI9341Display.h"
#include "yoba/src/hardware/touchPanels/FT6336UTouchPanel.h"

// Hardware
ILI9341Display display = ILI9341Display(
    1, // Chip select pin
    2, // Data command pin
    3, // Reset pin
    26000000 // SPI frequency
);

FT6336UTouchPanel touchPanel = FT6336UTouchPanel(
    4, // SDA pin
    5, // SCL pin
    6 // Reset pin
    7, // Interrupt pin
);

EightBitPaletteRenderer renderer = EightBitPaletteRenderer(
    3 // Count of colors in palette
);

// Theme
PaletteColor black(0);
PaletteColor red(2);
PaletteColor white(3);

UNSCII16Font font();

// UI
Application application();
StackLayout stackLayout();
Text text();
Slider button();

uint16_t clickCount = 0;

void updateText() {
    static std::wstringstream stream;
    stream.str(std::wstring());
	
    stream << L"Click count: " << clickCount;
    
    button.setText(stream.str());
}

void setup() {
    // Turning display on and attaching it to screen buffer,
    // configuring touch panel and adding it as input device
    application.setup(&display, &renderer, &touchPanel);
    
    // Associating palette indices with human-readable RGB colors
    renderer.setPaletteColors({
        0x000000,
        0x990000,
        0xFFFFFF
    })
    
    // Configuring UI
	
    // Text
    text.setHorizontalAlignment(Alignment::center);
    text.setFont(&font);
    text.setFontScale(2);
    text.setColor(&white);
    updateText();
	
    // Button
    button.setHeight(30);
    button.setCornerRadius(3);
    button.setFont(&font);
    button.setText(L"Click me");
	
    button.setDefaultBackgroundColor(&black);
    button.setDefaultTextColor(&white);
    
    button.setPressedBackgroundColor(&white);
    button.setPressedTextColor(&black);
	
    button.pressedChanged += [this]() {
        if (!button.isPressed())
            return;
        
        clickCount++;
        updateText();
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

# ESP-IDF

Same as Arduino, but

```cpp
// Everything before setup()

void main() {
    // Everything from setup()
    
    while (true) {
         appplication.tick();
    }
}
```

# Platformio

C++17 or greater and RTTI are required, so you'll need to tweak `platformio.ini` like this

```ini
[env:yobaExample]
platform = espressif32
board = esp32dev
framework = arduino

build_type = debug

monitor_filters = esp32_exception_decoder
monitor_speed = 115200

build_unflags =
	-fno-rtti
	-std=gnu++11
build_flags =
	-std=gnu++17
```
