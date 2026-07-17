#include <cstdio>
#include <cstring>

#include <freertos/FreeRTOS.h>
#include <freertos/task.h>
#include <esp_timer.h>

#include <YOBA/Core.hpp>
#include <YOBA/Rendering.hpp>
#include <YOBA/UI.hpp>

#include <YOBA/Hardware/Displays/ILI9341Display.hpp>
#include <YOBA/Resources/Fonts/Unscii16Font.hpp>

using namespace YOBA;

// Choosing display driver. Each display has its own register map, resolution & color modes, so
ILI9341Display display {};

// Choosing renderer that is suitable for our display. RGB565 is a good compromise between RAM/performance,
// in our case it will allocate 320 * 240 * 2 = 150 KiB of heap for pixel buffer
RGB565TransactionalBufferedRenderer renderer {};

// Using one of default fonts
constexpr static Unscii16Font font {};
constexpr static uint8_t fontScale = 3;

// Defining some colors
constexpr static RGB565Color backgroundColor = RGB888Color(0xFFFFFF).toRGB565();
constexpr static RGB565Color textColor = RGB888Color(0x000000).toRGB565();

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
        renderer.putText(
            display.getSize().getCenter() - font.getSize(fontScale, text).getCenter(),
            &font,
            fontScale,
            &textColor,
            text
        );

        // Flushing pixel buffer into display
        renderer.flush();

        // Waiting for a while to achieve ~30 FPS
        vTaskDelay(pdMS_TO_TICKS(1000 / 30));
    }
}