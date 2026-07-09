
# YOBA | Your Breathtaking Application

An independent and hardcore UI framework written on modern C++ for embedded devices

# Features

- Classic OOP-based approach without bullshit
- Tons of components like buttons, sliders, switches, text fields & scroll views
- Fully automatic layouts
- Animations & rendering transforms
- Event system for external peripherals like touch screens & keyboards with possibility to create custom events
- Compact image & font format with a nice [tool](https://github.com/IgorTimofeev/YobaResourceConverter)
to convert your dick pics into production-ready projects
- A clear distinction between UI, renderers and rendering targets (screens, desktop windows, etc.),
all of which can be used separately
- Monochrome, RGB 565/666/888 & indexed colors support
- Out-of-box drivers for the most popular displays like ILI9341, ST7789, ST7565, GC9A01, and SH1106
- HAL for easy integration of third-party MCUs
- [SFML](https://github.com/sfml/sfml) support for cross-platform desktop testing

# Showcase

<img width="320" src="https://github.com/user-attachments/assets/00eda8cc-0ebe-4b26-ac73-c90a9fe2989c" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/2a639a4d-81ce-419f-bcd4-6178148a0e56" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/e161327c-f554-4642-8cea-95da666df3de" alt=".!."/>

<img width="320" src="https://github.com/user-attachments/assets/0005fa8d-2c2a-4fc8-a503-70154f87916f" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/7675db07-ebf1-4d8d-9e49-9c949582b9e3" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/22ea362e-30ea-4723-b947-de339feda69b" alt=".!."/>

<img width="320" src="https://github.com/user-attachments/assets/33cd6ce1-6bd5-45f8-8d2a-9499d2e68c04" alt=".!."/>
<img width="320" src="https://github.com/user-attachments/assets/fff71020-69b0-4988-950f-6b571254c380" alt=".!."/>

# ESP-IDF installation

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
    "*.cpp"
)

idf_component_register(
    SRCS ${my_sources}
    INCLUDE_DIRS "."
    REQUIRES
        YOBA
)

include_directories(.)
```

# Desktop installation

Since `YOBA` is hardware-independent, I thought it would be fun to add support for running it on Windows and Linux.
And [SFML](https://github.com/sfml/sfml) is perfect for such shit!
Just clone an [example project](https://github.com/IgorTimofeev/YOBASFMLExample) and run it with your favourite IDE:

`git clone https://github.com/IgorTimofeev/YOBASFMLExample.git`

Alternatively, you can add `YOBA` as dependency by yourself:

`git submodule add https://github.com/IgorTimofeev/YOBA.git lib/YOBA`

Next, configure your` CMakeLists.txt` so that it looks something like this:

```cmake
cmake_minimum_required(VERSION 3.28)
project(YOBADesktopDemo LANGUAGES CXX)

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)
set(CMAKE_CXX_STANDARD 23)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

# Main
file(GLOB_RECURSE MAIN_SOURCES
    "src/*.cpp"
)

add_executable(main ${MAIN_SOURCES})
target_compile_features(main PRIVATE cxx_std_23)

# SFML
include(FetchContent)
FetchContent_Declare(
    SFML
    GIT_REPOSITORY https://github.com/SFML/SFML.git
    GIT_TAG 3.1.0
    GIT_SHALLOW ON
    EXCLUDE_FROM_ALL
    SYSTEM
)
FetchContent_MakeAvailable(SFML)

target_link_libraries(main PRIVATE SFML::Graphics)

# YOBA
add_subdirectory(lib/YOBA)
target_link_libraries(main PRIVATE YOBA)

set_property(DIRECTORY ${CMAKE_CURRENT_SOURCE_DIR} PROPERTY VS_STARTUP_PROJECT main)

```


# Tselyebnov, M. D.
<img width="1200" alt="517616031-76ed9af3-53a3-4d2d-b944-3c228edfec81-1" src="https://github.com/user-attachments/assets/09623ca2-fe56-4cd6-82f6-25493bbd022c"/>
