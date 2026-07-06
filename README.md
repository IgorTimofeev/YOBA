
# YOBA | Your Breathtaking Application

An independent and hardcore UI framework written on modern C++ for embedded devices

# Features

- Classic OOP-based approach without bullshit
- Tons of components like buttons, sliders, switches, text fields & scroll views
- Fully automatic layouts
- Animations & rendering transforms
- Event system for external peripherals like touch screens & keyboards with possibility to create custom ones
- Compact image & font format with a nice [tool](https://github.com/IgorTimofeev/YobaResourceConverter)
to convert your dick pics into production-ready projects
- A clear distinction between UI, renderers and rendering targets (screens, desktop windows, etc.),
all of which can be used separately
- Monochrome, RGB 565/666/888 & indexed colors support
- Out-of-box drivers for the most popular displays like ILI9341, ST7789, ST7565, GC9A01, and SH1106
- HAL for any MCU integration

# Showcase

<img width="300" src="https://github.com/user-attachments/assets/00eda8cc-0ebe-4b26-ac73-c90a9fe2989c" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/2a639a4d-81ce-419f-bcd4-6178148a0e56" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/e161327c-f554-4642-8cea-95da666df3de" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/0005fa8d-2c2a-4fc8-a503-70154f87916f" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/7675db07-ebf1-4d8d-9e49-9c949582b9e3" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/22ea362e-30ea-4723-b947-de339feda69b" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/33cd6ce1-6bd5-45f8-8d2a-9499d2e68c04" alt=".!."/>
<img width="300" src="https://github.com/user-attachments/assets/fff71020-69b0-4988-950f-6b571254c380" alt=".!."/>

# Installation on ESP-IDF:

First, you must enable RTTI (run-time type information), since complex components like keyboard and
selectors use `dynamic_cast`. RTTI is disabled by default on ESP-IDF to save some flash memory,
but since we’re using a fucking UI framework, a few bytes is a ridiculously small price
to pay for such immense power. RTTI itself can be enabled via

`idf.py menuconfig` > `Compiler options` > `Enable C++ run-time type info (RTTI)`

After that, you can clone the library into your project. It would be wise to use submodules to do this:

`git submodule add https://github.com/IgorTimofeev/YOBA.git components/YOBA`

Don't forget to add `YOBA` component to your `CMakeLists.txt`. It should look like this:

```cmake
file(GLOB_RECURSE my_sources
    "*.*"
)

idf_component_register(
    SRCS ${my_sources}
    INCLUDE_DIRS "."
    REQUIRES
        YOBA
)

include_directories(.)
```

# Code examples

### Direct rendering

You can easily reject all these trendy OOP features and render your interface directly with minimal overhead.
Of course this approach is equivalent to voluntary castration for complex UIs, but it's perfectly reasonable
for simple applications:

```c++
#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>

#include <YOBA/core.h>
#include "YOBA/rendering.h"
#include <YOBA/UI.h>

#include <YOBA/hardware/displays/ILI9341Display.h>
#include <YOBA/resources/fonts/unscii16Font.h>

using namespace YOBA;

// Choosing display driver. Each display has its own register map, resolution & color modes, so
ILI9341Display display {};

// Choosing renderer that is suitable for our display. RGB565 is a good compromise between RAM/performance,
// in our case it will allocate 320 * 240 * 2 = 150 KiB of heap for pixel buffer
RGB565Renderer renderer {};

// Using one of default fonts
constexpr static Unscii16Font font {};
constexpr static uint8_t fontScale = 3;

// Defining some colors
constexpr static RGB565Color backgroundColor = RGB888Color(0xFFFFFF).toRGB565BE();
constexpr static RGB565Color textColor = RGB888Color(0x000000).toRGB565BE();

extern "C" void app_main(void) {
    // Initializing display - replace GPIOs with yours
    display.setup(
		GPIO_NUM_23,
		GPIO_NUM_18,
		GPIO_NUM_17,
		GPIO_NUM_16,
		GPIO_NUM_NC,
		60'000'000,

		Size(240, 320),
		Rotation::none,
		ColorModel::RGB565
    );

    // Assigning display as rendering target
    renderer.setTarget(&display);

    // By default, display is turned off to prevent random memory garbage to be shown
    // So we should at least turn it on
    display.turnOn();

    while (true) {
        // Clearing display with background color
        renderer.clear(&backgroundColor);

        // Creating a formatted string with current uptime like "00:12"
        const auto uptimeMicroseconds = esp_timer_get_time();
        const auto uptimeMinutes = uptimeMicroseconds / 60'000'000;
        const auto uptimeSeconds = uptimeMicroseconds % 60'000'000 / 1'000'000;

        char text[24];
        std::snprintf(text, sizeof(text), "%02lld:%02lld", uptimeMinutes, uptimeSeconds);

        // Rendering time on center of display
        renderer.renderText(
            display.getSize().getCenter() - font.getSize(text, fontScale).getCenter(),
            &font,
            &textColor,
            text,
            fontScale
        );

        // Flushing pixel buffer into display
        renderer.flush();

        // Waiting for a while to achieve ~30 FPS
        vTaskDelay(pdMS_TO_TICKS(1000 / 30));
    }
}
```

### Result

<img width="300" src="https://github.com/user-attachments/assets/8e86f467-7826-44c1-a4ab-6aca1dbe8930" alt=".!."/>

# Tselyebnov, M. D.
<img width="1200" alt="517616031-76ed9af3-53a3-4d2d-b944-3c228edfec81-1" src="https://github.com/user-attachments/assets/09623ca2-fe56-4cd6-82f6-25493bbd022c"/>
