
# YOBA | Your Breathtaking Application

An independent and hardcore UI framework written on modern C++ for embedded devices.

# Features

- Classic OOP-based approach without bullshit
- Tons of components like buttons, sliders, switches, text fields & scroll views
- Fully automatic layouts
- Animations & rendering transforms
- Event system for external peripherals like touch screens & keyboards with possibility to create custom ones
- Compact image & font format with a nice [tool](https://github.com/IgorTimofeev/YobaResourceConverter)
to convert your dick pics into production-ready projects
- A clear distinction between UI, renderers and rendering targets (screens, desktop windows, etc.),
all of which can be used independently
- Monochrome, RGB 565/666/888 & indexed colors support
- Out-of-box drivers for the most popular displays like ILI9341, ST7789, ST7565, GC9A01, and SH1106
- HAL for any MCU integration

# Installation

## ESP-IDF:

First, you must enable RTTI (run-time type information), since complex components like combo boxes and
selectors use `dynamic_cast`. On ESP-IDF RTTI is disabled by default to save some flash memory,
but since we’re using an entire UI framework, a few bytes is a ridiculously small price
to pay for such immense power. RTTI itself can be enabled via 


`idf.py menuconfig` > `Compiler options` > `Enable C++ run-time type info (RTTI)`

After this you can clone library and integrate it into your project.
The wise path to do this is using submodules:

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

# Showcase

<img width="400" src="https://github.com/user-attachments/assets/00eda8cc-0ebe-4b26-ac73-c90a9fe2989c"/>
<img width="400" src="https://github.com/user-attachments/assets/2a639a4d-81ce-419f-bcd4-6178148a0e56"/>
<img width="400" src="https://github.com/user-attachments/assets/e161327c-f554-4642-8cea-95da666df3de"/>
<img width="400" src="https://github.com/user-attachments/assets/0005fa8d-2c2a-4fc8-a503-70154f87916f"/>
<img width="400" src="https://github.com/user-attachments/assets/7675db07-ebf1-4d8d-9e49-9c949582b9e3"/>
<img width="400" src="https://github.com/user-attachments/assets/22ea362e-30ea-4723-b947-de339feda69b"/>
<img width="400" src="https://github.com/user-attachments/assets/33cd6ce1-6bd5-45f8-8d2a-9499d2e68c04"/>
<img width="400" src="https://github.com/user-attachments/assets/fff71020-69b0-4988-950f-6b571254c380"/>

# Tselyebnov, M. D.
<img width="2400" height="2400" alt="517616031-76ed9af3-53a3-4d2d-b944-3c228edfec81-1" src="https://github.com/user-attachments/assets/09623ca2-fe56-4cd6-82f6-25493bbd022c"/>
