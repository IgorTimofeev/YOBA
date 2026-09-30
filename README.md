
# YOBA | Your Breathtaking Application

An independent and hardcore UI framework written on modern C++ for embedded devices

# Features

- Classic OOP-based approach without bullshit
- Tons of components like buttons, sliders, switches, spinners, text fields & scroll views
- Fully automatic layouts
- Animations & rendering transforms
- Event system for external peripherals like touch screens & keyboards
- Compact image & font format with a nice [tool](https://github.com/IgorTimofeev/YOBAResourceConverter)
to convert your dick pics into production-ready projects
- A clear distinction between UI, renderers and rendering targets (screens, desktop windows, etc.),
all of which can be used separately
- Monochrome, RGB 565/666/888, ARGB & indexed colors support
- Out-of-box drivers for the most popular displays like ILI9341, ST7789, ST7565, GC9A01, and SH1106
- HAL for easy integration of third-party MCUs
- [SFML](https://github.com/sfml/sfml) support for cross-platform testing

# Showcase

<img width="320" src="https://github.com/user-attachments/assets/00eda8cc-0ebe-4b26-ac73-c90a9fe2989c" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/2a639a4d-81ce-419f-bcd4-6178148a0e56" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/e161327c-f554-4642-8cea-95da666df3de" alt=".!."/>

<img width="320" src="https://github.com/user-attachments/assets/0005fa8d-2c2a-4fc8-a503-70154f87916f" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/7675db07-ebf1-4d8d-9e49-9c949582b9e3" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/22ea362e-30ea-4723-b947-de339feda69b" alt=".!."/>

# ESP-IDF

## Installation

Clone the library into your project's `components` directory:

`git clone https://github.com/IgorTimofeev/YOBA.git components/YOBA`

Then add `YOBA` component into `main/CMakeLists.txt`. It should look like this:

```cmake
idf_component_register(
    SRCS main.cpp
    INCLUDE_DIRS "."
    # Here
    REQUIRES YOBA
)
```

## Examples

<img width="240" alt="image" src="https://github.com/user-attachments/assets/e7c46649-bc23-43fa-9c38-e751a7b3c4d5" />

### [Direct rendering](https://github.com/IgorTimofeev/YOBA-ESP-IDF-direct-rendering-example)

A demonstration of displaying formatted time without using OOP. Suitable for simple projects such as timers, static images, etc.

# Desktop

Since `YOBA` is hardware-independent, I thought it would be fun to add support for running it on Windows and Linux.
And [SFML](https://github.com/sfml/sfml) is perfect for such shit!

## Installation

Clone the library into any directory of your project like `lib` or whatever:

`git clone https://github.com/IgorTimofeev/YOBASFMLExample.git lib/YOBA`

Then add `YOBA` into your `main/CMakeLists.txt`. It should look like this:

```cmake
...

# YOBA
add_subdirectory(lib/YOBA)
target_link_libraries(${PROJECT_NAME} PRIVATE YOBA)
```

## Examples

<img width="240" alt="image" src="https://github.com/user-attachments/assets/dc882fd1-bc89-48b7-a6d3-7e8b2f1e1085" />

### [Button and text](https://github.com/IgorTimofeev/YOBA-SFML-button-example)

Simple push button that increases dick size

<img width="240" alt="image" src="https://github.com/user-attachments/assets/71d174fa-0d44-466a-884c-694100eaffe9" />

### [Advanced](https://github.com/IgorTimofeev/YOBA-SFML-full-example)

Themes, resources, all available controls - every darkest desire in one project. Dark souls lvl of understanding required

# Tselyebnov, M. D.
<img width="1200" alt="feet" src="https://github.com/user-attachments/assets/09623ca2-fe56-4cd6-82f6-25493bbd022c"/>